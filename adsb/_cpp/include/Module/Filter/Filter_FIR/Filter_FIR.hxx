/*!
 * \file
 * \brief Filters a signal.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef FILTER_FIR_HXX_
#define FILTER_FIR_HXX_

#include <string>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <sstream>

#include "mipp.h"
#include "Module/Filter/Filter_FIR/Filter_FIR.hpp"

namespace aff3ct
{
namespace module
{

template <typename R, bool complex_data>
Filter_FIR<R, complex_data>
::Filter_FIR(const int N, const std::vector<R> h)
: Filter<R>((complex_data ? 2 : 1) * N, (complex_data ? 2 : 1) * N),
mem(2 * (complex_data ? 2 : 1) * h.size(), R(0)),
head(0),
size((int)h.size()),
M(mipp::N<R>()),
P(complex_data ? (2*N-2*(h.size()-1))/mipp::N<R>(): (N-(h.size()-1))/mipp::N<R>()),
f_h(h.size(), R(0))
{
	assert(size > 0);
	assert(mipp::N<R>() > 1);
	
	if (P < 0)
		P = 0;

	for (size_t i = 0; i < f_h.size(); i++)
		this->f_h[i] = h[h.size() - 1 - i];
}

template <typename R, bool complex_data>
void Filter_FIR<R, complex_data>
::_filter_c(const R *X_N1, R *Y_N2, const int frame_id)
{
	auto cX_N1 = reinterpret_cast<const std::complex<R>* >(X_N1);
	auto cY_N2 = reinterpret_cast<      std::complex<R>* >(Y_N2);

	int rest = this->N - this->P * this->M;

	for(auto i = 0; i < rest/2; i++)
		step(&cX_N1[i], &cY_N2[i]);

	mipp::Reg<R> ps = (R)0;
	mipp::Reg<R> ps0;
	mipp::Reg<R> ps1;
	mipp::Reg<R> ps2;
	mipp::Reg<R> ps3;

	mipp::Reg<R> reg_x0;
	mipp::Reg<R> reg_x1;
	mipp::Reg<R> reg_x2;
	mipp::Reg<R> reg_x3;

	mipp::Reg<R> reg_fh0;
	mipp::Reg<R> reg_fh1;
	mipp::Reg<R> reg_fh2;
	mipp::Reg<R> reg_fh3;

	size_t size = this->size;
	size_t b_size_unrolled4 = (size / 4) * 4;

	for (auto i = rest; i < this->N ; i += this->M)
	{
		ps0 = (R)0;
		ps1 = (R)0;
		ps2 = (R)0;
		ps3 = (R)0;

		for (size_t k = 0; k < b_size_unrolled4; k += 4)
		{
			reg_fh0 = f_h[k +0];
			reg_fh1 = f_h[k +1];
			reg_fh2 = f_h[k +2];
			reg_fh3 = f_h[k +3];

			reg_x0 = &X_N1[-2 * (size -1) + i + 2 * (k + 0)];
			reg_x1 = &X_N1[-2 * (size -1) + i + 2 * (k + 1)];
			reg_x2 = &X_N1[-2 * (size -1) + i + 2 * (k + 2)];
			reg_x3 = &X_N1[-2 * (size -1) + i + 2 * (k + 3)];

			ps0 = mipp::fmadd(reg_fh0, reg_x0, ps0); // same as 'ps0 += reg_fh0 * reg_x0'
			ps1 = mipp::fmadd(reg_fh1, reg_x1, ps1); // same as 'ps1 += reg_fh1 * reg_x1'
			ps2 = mipp::fmadd(reg_fh2, reg_x2, ps2); // same as 'ps2 += reg_fh2 * reg_x2'
			ps3 = mipp::fmadd(reg_fh3, reg_x3, ps3); // same as 'ps3 += reg_fh3 * reg_x3'
		}

		ps0 += ps1;
		ps2 += ps3;
		ps = ps0 + ps2;

		for (size_t k = b_size_unrolled4; k < size; k++)
		{
			reg_fh0 = f_h[k];
			reg_x0.load(X_N1 - 2 * (size -1) + i + (2 * k));
			ps = mipp::fmadd(reg_fh0, reg_x0, ps); // same as 'ps += reg_fh0 * reg_x0'
		}

		ps.store(Y_N2 + i);
	}

	int sz = this->N;
	std::copy(&X_N1[sz - 2*this->size], &X_N1[sz], &this->mem[0]);
	std::copy(&X_N1[sz - 2*this->size], &X_N1[sz], &this->mem[2*this->size]);
	this->head = 0;
}

template <typename R, bool complex_data>
void Filter_FIR<R, complex_data>
::_filter_r(const R *X_N1, R *Y_N2, const int frame_id)
{
	int rest = this->N - this->P * this->M;

	for(auto i = 0; i < rest; i++)
		step(&X_N1[i], &Y_N2[i]);

	mipp::Reg<R> ps = (R)0;
	mipp::Reg<R> ps0;
	mipp::Reg<R> ps1;
	mipp::Reg<R> ps2;
	mipp::Reg<R> ps3;

	mipp::Reg<R> reg_x0;
	mipp::Reg<R> reg_x1;
	mipp::Reg<R> reg_x2;
	mipp::Reg<R> reg_x3;

	mipp::Reg<R> reg_fh0;
	mipp::Reg<R> reg_fh1;
	mipp::Reg<R> reg_fh2;
	mipp::Reg<R> reg_fh3;
	
	size_t size = this->size;
	size_t b_size_unrolled4 = (size / 4) * 4;

	for (auto i = rest; i < this->N ; i += this->M)
	{
		ps0 = (R)0;
		ps1 = (R)0;
		ps2 = (R)0;
		ps3 = (R)0;

		for (size_t k = 0; k < b_size_unrolled4; k += 4)
		{
			reg_fh0 = f_h[k +0];
			reg_fh1 = f_h[k +1];
			reg_fh2 = f_h[k +2];
			reg_fh3 = f_h[k +3];

			reg_x0 = &X_N1[- (size -1) + i +  (k + 0)];
			reg_x1 = &X_N1[- (size -1) + i +  (k + 1)];
			reg_x2 = &X_N1[- (size -1) + i +  (k + 2)];
			reg_x3 = &X_N1[- (size -1) + i +  (k + 3)];

			ps0 = mipp::fmadd(reg_fh0, reg_x0, ps0); // same as 'ps0 += reg_fh0 * reg_x0'
			ps1 = mipp::fmadd(reg_fh1, reg_x1, ps1); // same as 'ps1 += reg_fh1 * reg_x1'
			ps2 = mipp::fmadd(reg_fh2, reg_x2, ps2); // same as 'ps2 += reg_fh2 * reg_x2'
			ps3 = mipp::fmadd(reg_fh3, reg_x3, ps3); // same as 'ps3 += reg_fh3 * reg_x3'
		}

		ps0 += ps1;
		ps2 += ps3;
		ps = ps0 + ps2;

		for (size_t k = b_size_unrolled4; k < size; k++)
		{
			reg_fh0 = f_h[k];
			reg_x0.load(X_N1 - (size -1) + i + k);
			ps = mipp::fmadd(reg_fh0, reg_x0, ps); // same as 'ps += reg_fh0 * reg_x0'
		}

		ps.store(Y_N2 + i);
	}

	int sz = this->N;
	std::copy(&X_N1[sz - this->size], &X_N1[sz], &this->mem[0]);
	std::copy(&X_N1[sz - this->size], &X_N1[sz], &this->mem[this->size]);
	this->head = 0;
}

template <typename R, bool complex_data>
void Filter_FIR<R, complex_data>
::_filter(const R *X_N1,  R *Y_N2, const int frame_id)
{
	if constexpr (complex_data)
		_filter_c(X_N1, Y_N2, frame_id);
	else
		_filter_r(X_N1, Y_N2, frame_id);
}

template <typename R, bool complex_data>
void Filter_FIR<R, complex_data>::reset()
{

	for (size_t i = 0; i < this->mem.size(); i++)
		this->mem[i] = R(0);

	this->head = 0;
}

template <typename R, bool complex_data>
std::vector<R> Filter_FIR<R, complex_data>::get_h()
{
	std::vector<R> h(this->f_h.size(),(R)0);
	for (size_t i=0; i < this->f_h.size(); i++)
		h[i] = this->f_h[this->f_h.size()-1-i];

	return h;
}


}
}

#endif
