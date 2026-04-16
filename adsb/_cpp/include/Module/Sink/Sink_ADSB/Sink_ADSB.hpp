/*!
 * \file
 * \brief Send data to ADSB server.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SINK_ADSB_HPP
#define SINK_ADSB_HPP

#include <streampu.hpp>

namespace aff3ct
{
namespace module
{


/*!
 * \class Operation
 *
 * \brief Transmit or receive data to or from a radio module.
 *
 * \tparam R: type of the data to send or receive.
 *
 */
template <typename B = int32_t>
class Sink_ADSB : public spu::module::Sink<B>
{
public:
	/*!
	 * \brief Constructor.
	 *
	 * \param user_name: User name for the connection.
	 * \param server_addr: Server address.
	 * \param server_port: Server port.
	 */
	Sink_ADSB(const std::string& user_name, const std::string& server_addr, const int server_port, const int max_queue_size);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Sink_ADSB() = default;

	/*!
	 * \brief Flush the sink.
	 *
	 * \return int: status code.
	 */
	int flush();

	/*!
	 * \brief Print the message queue.
	 */
	void print_queue();
protected:
	std::string user_name;
	std::string server_addr;
	int         server_port;

	std::chrono::steady_clock::time_point last_packet_time;
	std::vector<nlohmann::json> message_queue;
	int max_queue_size;
	std::atomic<size_t> read_index;
	std::atomic<size_t> write_index;
	int get_queue_size();
	void enqueue_message(const std::string& message);
	int dequeue_message(nlohmann::json& msg);
	int dequeue_n_messages(nlohmann::json& msgs, int msg_nbr);
	
	void _send_count(const B* in_data, const uint32_t* in_count, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Sink/Sink_ADSB/Sink_ADSB.hxx"
#endif
#endif /* SINK_ADSB_HPP */
