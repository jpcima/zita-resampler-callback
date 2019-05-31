//          Copyright Jean Pierre Cimalando 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "zita-resampler-callback.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

template <class Rs>
BasicResamplerByCallback<Rs>::BasicResamplerByCallback()
    : ratio_(), gen_(), cookie_(), buf_(), buf_size_(), buf_fill_()
{
}

template <class Rs>
BasicResamplerByCallback<Rs>::~BasicResamplerByCallback()
{
    clear();
}

template <class Rs>
void BasicResamplerByCallback<Rs>::clear()
{
    free(buf_);
    buf_ = 0;
    resampler_.clear();
}

template <class Rs>
int BasicResamplerByCallback<Rs>::reset()
{
    buf_fill_ = 0;
    return resampler_.reset();
}

template <class Rs>
int BasicResamplerByCallback<Rs>::generate(float *frames, unsigned nframes)
{
    Rs &resampler = resampler_;
    double ratio = ratio_;
    unsigned nchan = resampler.nchan();
    float *buf = buf_;
    unsigned bsize = buf_size_;
    unsigned bfill = buf_fill_;
    generator *gen = gen_;
    void *cookie = cookie_;

    while (nframes > 0) {
        unsigned in_need = (unsigned)ceil(nframes * (1.0 / ratio));
        in_need = (in_need < bsize) ? in_need : bsize;

        if (bfill < in_need) {
            gen(cookie, buf + nchan * bfill, in_need - bfill);
            bfill = in_need;
        }

        resampler.inp_count = bfill;
        resampler.inp_data = buf;
        resampler.out_count = nframes;
        resampler.out_data = frames;

        int ret = resampler.process();
        if (ret != 0)
            return ret;

        unsigned in_remaining = resampler.inp_count;
        memmove(buf, buf + nchan * (bfill - in_remaining), in_remaining * nchan * sizeof(float));
        bfill = in_remaining;

        unsigned out_remaining = resampler.out_count;
        frames += nframes - out_remaining;
        nframes = out_remaining;
    }

    buf_fill_ = bfill;
    return 0;
}

template <class Rs>
int BasicResamplerByCallback<Rs>::setup_(double ratio, generator *gen, void *cookie, unsigned bsize)
{
    Rs &resampler = resampler_;
    unsigned nchan = resampler.nchan();

    bsize = (bsize < 16) ? 16 : bsize;
    float *buf = (float *)malloc(nchan * bsize * sizeof(float));
    if (!buf)
        return 1;

    ratio_ = ratio;
    gen_ = gen;
    cookie_ = cookie;
    buf_ = buf;
    buf_size_ = bsize;
    buf_fill_ = 0;

    return 0;
}

int ResamplerByCallback::setup(double fs_inp, double fs_out, unsigned nchan, unsigned hlen, generator *gen, void *cookie, unsigned bsize)
{
    Resampler &resampler = resampler_;
    double ratio = fs_out / fs_inp;

    int ret = resampler.setup(fs_inp, fs_out, nchan, hlen);
    if (ret != 0)
        return ret;

    ret = setup_(ratio, gen, cookie, bsize);
    if (ret != 0) {
        resampler.clear();
        return ret;
    }

    return 0;
}

int ResamplerByCallback::setup(double fs_inp, double fs_out, unsigned nchan, unsigned hlen, double frel, generator *gen, void *cookie, unsigned bsize)
{
    Resampler &resampler = resampler_;
    double ratio = fs_out / fs_inp;

    int ret = resampler.setup(fs_inp, fs_out, nchan, hlen, frel);
    if (ret != 0)
        return ret;

    ret = setup_(ratio, gen, cookie, bsize);
    if (ret != 0) {
        resampler.clear();
        return ret;
    }

    return 0;
}

int VResamplerByCallback::setup(double ratio, unsigned int nchan, unsigned int hlen, generator *gen, void *cookie, unsigned bsize)
{
    VResampler &resampler = resampler_;

    int ret = resampler.setup(ratio, nchan, hlen);
    if (ret != 0)
        return ret;

    ret = setup_(ratio, gen, cookie, bsize);
    if (ret != 0) {
        resampler.clear();
        return ret;
    }

    return 0;
}

int VResamplerByCallback::setup(double ratio, unsigned int nchan, unsigned int hlen, double frel, generator *gen, void *cookie, unsigned bsize)
{
    VResampler &resampler = resampler_;

    int ret = resampler.setup(ratio, nchan, hlen, frel);
    if (ret != 0)
        return ret;

    ret = setup_(ratio, gen, cookie, bsize);
    if (ret != 0) {
        resampler.clear();
        return ret;
    }

    return 0;
}

template class BasicResamplerByCallback<Resampler>;
template class BasicResamplerByCallback<VResampler>;
