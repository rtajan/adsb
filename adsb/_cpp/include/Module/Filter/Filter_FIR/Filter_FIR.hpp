#ifndef FILTER_FIR_HPP
#define FILTER_FIR_HPP

#include <vector>
#include <complex>

#include "Module/Filter/Filter.hpp"

namespace aff3ct
{
namespace module
{
/**
 * @brief Finite Impulse Response (FIR) filter
 * 
 * @tparam R The data type (float or double)
 */
template <typename R = float, bool complex_data = false>
class Filter_FIR : public Filter<R>
{
protected:
	std::vector<R> mem; // internal memory of the filter as real values
	int head;
	int size;
	int M;
	int P;

	// Methods to filter one sample
	// They are inlined for performance reasons
	// (to avoid the function call overhead)
	// They are defined at the end of this file
	inline void step(const std::complex<R>* x_elt, std::complex<R>* y_elt);
	inline void step(const R* x_elt, R* y_elt);

public:

	/**
	 * @brief Construct a new Filter_FIR object
	 * 
	 * @param N The number of samples at filter's input/output
	 * @param h The filter coefficients
	 * @param complex_input Whether the input is complex or not
	 */
	Filter_FIR (const int N, const std::vector<R> h);
	virtual ~Filter_FIR() = default;
	
	void reset();
	std::vector<R> get_h(); // get the filter coefficients

protected:
	std::vector<R> f_h; // filter coefficients (flipped)
	void _filter_r(const R *X_N1,  R *Y_N2, const int frame_id); // filter for real data
	void _filter_c(const R *X_N1,  R *Y_N2, const int frame_id); // filter for complex data

	void _filter(const R *X_N1,  R *Y_N2, const int frame_id); // generic filter call for task
};

// Adrien: I put this function here because you wanted to be inlined
template <typename R, bool complex_data>
void Filter_FIR<R, complex_data>
::step(const std::complex<R>* x_elt, std::complex<R>* y_elt)
{
	std::complex<R>* mem_c = reinterpret_cast<std::complex<R>*>(this->mem.data());
	mem_c[this->head] = *x_elt;
	mem_c[this->head + this->size] = *x_elt;

	*y_elt = mem_c[this->head+1] * this->f_h[0];
	for (auto i = 1; i < this->size ; i++)
		*y_elt += mem_c[this->head + 1 + i] * this->f_h[i];

	this->head++;
	this->head %= this->size;
}

// Adrien: I put this function here because you wanted to be inlined
template <typename R, bool complex_data>
void Filter_FIR<R, complex_data>
::step(const R* x_elt, R* y_elt)
{
	this->mem[this->head] = *x_elt;
	this->mem[this->head + this->size] = *x_elt;

	*y_elt = this->mem[this->head+1] * this->f_h[0];
	for (auto i = 1; i < this->size ; i++)
		*y_elt += this->mem[this->head + 1 + i] * this->f_h[i];

	this->head++;
	this->head %= this->size;
}


}
}
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Filter_FIR.hxx"
#endif
#endif //FILTER_FIR_HPP
