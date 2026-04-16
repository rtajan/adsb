/*!
 * \file
 * \brief Send data to ADSB server.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SINK_ADSB_HXX_
#define SINK_ADSB_HXX_

#include <sstream>
#include <complex>
#include <csignal>

#include "Module/Sink/Sink_ADSB/Sink_ADSB.hpp"
#include "nlohmann/json.hpp"
#include <curl/curl.h>

// Callback function to write CURL response data
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


namespace aff3ct
{
namespace module
{

template <typename B>
Sink_ADSB<B>
::Sink_ADSB(const std::string& user_name, const std::string& server_addr, const int server_port, const int max_queue_size)
: spu::module::Sink<B>(112),
  user_name(user_name),
  server_addr(server_addr),
  server_port(server_port),
  last_packet_time(std::chrono::steady_clock::now()),
  message_queue(max_queue_size),
  max_queue_size(max_queue_size),
  read_index(0),
  write_index(0)
  {
	const std::string name = "Sink_ADSB";
	this->set_name(name);
	this->set_short_name(name);
    if (user_name.empty())
    {
        std::stringstream message;
        message << "'user_name' cannot be empty.";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }
    if (server_addr.empty())
    {
        std::stringstream message;
        message << "'server_addr' cannot be empty.";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }
    if (server_port <= 0)
    {
        std::stringstream message;
        message << "'server_port' has to be greater than 0 ('server_port' = " << server_port << ").";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }
    if (server_port > 65535)
    {
        std::stringstream message;
        message << "'server_port' has to be less than or equal to 65535 ('server_port' = " << server_port << ").";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }

}

template<typename B>
void
Sink_ADSB<B>::_send_count(const B* in_data, const uint32_t* in_count, const size_t frame_id)
{
    std::stringstream hex_bits;

    for (int i = 0; i < 112; i+=4)
    {
        int val = in_data[i] * 8 + in_data[i+1] * 4 + in_data[i+2] * 2 + in_data[i+3];
        hex_bits << std::hex << std::uppercase << val;
    }

    this->enqueue_message(hex_bits.str());
}

template<typename B>
void
Sink_ADSB<B>::enqueue_message(const std::string& message)
{
    nlohmann::json msg = {{"username", this->user_name}, {"message", message}};
    if (this->get_queue_size() < this->max_queue_size) // Limit queue size to prevent overflow
    {
        message_queue[this->write_index].clear();
        message_queue[this->write_index] = msg;
        this->write_index++;
        while (this->write_index >= this->max_queue_size)
            this->write_index -= this->max_queue_size;

    }
    else
    {
        std::cerr << "Queue full, dropping message." << std::endl;
    }
}

template<typename B>
int
Sink_ADSB<B>::dequeue_message(nlohmann::json& msg)
{
    if (this->get_queue_size() > 0)
    {
        msg.clear();
        msg.update(this->message_queue[this->read_index]);
        this->read_index++;
        while (this->read_index >= this->max_queue_size)
            this->read_index -= this->max_queue_size;
        return 0;
    }
    return -1;
}

template<typename B>
int
Sink_ADSB<B>::dequeue_n_messages(nlohmann::json& msgs, int msg_nbr)
{
    if (this->get_queue_size() >= msg_nbr)
    {
        msgs.clear();
        msgs = nlohmann::json::array();
        for (int i = 0; i < msg_nbr; i++) {
            nlohmann::json single_msg;
            this->dequeue_message(single_msg);
            msgs.push_back(single_msg);
        }
        return 0;
    }
    return -1;
}

template<typename B>
int
Sink_ADSB<B>::get_queue_size()
{
    if (this->write_index >= this->read_index)
        return this->write_index - this->read_index;
    else
        return this->max_queue_size - (this->read_index - this->write_index);
}

template<typename B>
int
Sink_ADSB<B>::flush()
{

    if (std::chrono::steady_clock::now() - this->last_packet_time < std::chrono::milliseconds(500)) {
        return -1;// If less than 500ms since last packet, skip sending to avoid flooding
    }

    nlohmann::json message_to_server;
    int message_in_queue = this->get_queue_size();
    this->dequeue_n_messages(message_to_server["messages"], message_in_queue);

    int message_count = message_to_server["messages"].size();
    if (message_count == 0) {
        return 0; // No messages to send
    }

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        std::string url = "http://" + server_addr + ":" + std::to_string(server_port) + "/api/ingest";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        std::string json_str = message_to_server.dump();

        // std::cout << "Sending: " << json_str << std::endl;
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L); // no verbose
        std::string response_buffer;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_buffer);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    last_packet_time = std::chrono::steady_clock::now();
     return message_count;
}

template<typename B>
void
Sink_ADSB<B>::print_queue()
{
    int q_size = this->get_queue_size();
    std::cout << "Current queue size: " << q_size << std::endl;
    for (int i = 0; i < q_size; i++) {
        size_t index = (this->read_index + i) % this->max_queue_size;
        std::cout << message_queue[index].dump() << std::endl;
    }
}

}
}

#endif
