//          Copyright Jean Pierre Cimalando 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef ZITA_RESAMPLER_CALLBACK_H
#define ZITA_RESAMPLER_CALLBACK_H

#include <zita-resampler/resampler.h>
#include <zita-resampler/vresampler.h>

//------------------------------------------------------------------------------
template <class Rs>
class BasicResamplerByCallback
{
protected:
    BasicResamplerByCallback();
    ~BasicResamplerByCallback();

public:
    typedef void (generator)(void *cookie, float *frames, unsigned nframes);

    void clear();
    int reset();
    int nchan (void) const { return resampler_.nchan(); }
    int inpsize (void) const { return resampler_.inpsize(); }
    double inpdist (void) const { return resampler_.inpdist(); }
    int generate(float *frames, unsigned nframes);

protected:
    int setup_(double ratio, generator *gen, void *cookie, unsigned bsize);

protected:
    double ratio_;
    generator *gen_;
    void *cookie_;
    float *buf_;
    unsigned buf_size_;
    unsigned buf_fill_;
    Rs resampler_;
};

//------------------------------------------------------------------------------
class ResamplerByCallback : public BasicResamplerByCallback<Resampler>
{
public:
    ResamplerByCallback() {}
    ~ResamplerByCallback() {}

    int setup(double fs_inp, double fs_out, unsigned nchan, unsigned hlen, generator *gen, void *cookie, unsigned bsize);
    int setup(double fs_inp, double fs_out, unsigned nchan, unsigned hlen, double frel, generator *gen, void *cookie, unsigned bsize);
};

//------------------------------------------------------------------------------
class VResamplerByCallback : public BasicResamplerByCallback<VResampler>
{
public:
    VResamplerByCallback() {}
    ~VResamplerByCallback() {}

    int setup(double ratio, unsigned nchan, unsigned hlen, generator *gen, void *cookie, unsigned bsize);
    int setup(double ratio, unsigned nchan, unsigned hlen, double frel, generator *gen, void *cookie, unsigned bsize);
};

#endif // ZITA_RESAMPLER_CALLBACK_H
