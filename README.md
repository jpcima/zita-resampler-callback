# zita-resampler-callback
Callback-based API for Zita-resampler

# Usage

The API is a convenience wrapper around Zita-resampler.

Instead of filling a buffer with source rate signal, a callback is invoked as needed, and the buffering logic is handled transparently.

`ResamplerByCallback` and `VResamplerByCallback` are respective counterparts of `Resampler` and `VResampler`.

Setup parameters are:

| Parameter | Description |
| --------- | ----------- |
| generator *gen | callback function which produces source frames |
| void *cookie | user data to callback function |
| unsigned bsize | maximum frame count to request from the callback |
