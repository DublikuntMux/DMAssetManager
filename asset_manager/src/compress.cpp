#include <vector>

#ifdef ENABLE_DECODER
#include <brotli/decode.h>
#endif

#ifdef ENABLE_ENCODER
#include <brotli/encode.h>
#endif

#include "manager.hpp"

#ifdef ENABLE_DECODER
std::vector<char> decompress(const std::vector<char> &input)
{
  std::vector<char> output;

  BrotliDecoderState *state = BrotliDecoderCreateInstance(nullptr, nullptr, nullptr);

  size_t available_in = input.size();
  const uint8_t *next_in = reinterpret_cast<const uint8_t *>(input.data());

  size_t estimated_output_size = input.size() * 10;
  output.resize(estimated_output_size);

  size_t available_out = output.size();
  uint8_t *next_out = reinterpret_cast<uint8_t *>(output.data());

  BrotliDecoderResult result =
    BrotliDecoderDecompressStream(state, &available_in, &next_in, &available_out, &next_out, nullptr);

  if (result == BROTLI_DECODER_RESULT_SUCCESS) {
    output.resize(output.size() - available_out);
  } else {
    throw AssetMnagerException("Error: Brotli error.");
    output.clear();
  }

  BrotliDecoderDestroyInstance(state);

  return output;
}
#endif

#ifdef ENABLE_ENCODER
std::vector<char> compress(const std::vector<char> &input)
{
  std::vector<char> output;
  size_t output_size = BrotliEncoderMaxCompressedSize(input.size());
  output.resize(output_size);

  BrotliEncoderState *state = BrotliEncoderCreateInstance(nullptr, nullptr, nullptr);
  BrotliEncoderSetParameter(state, BROTLI_PARAM_QUALITY, 10);

  size_t available_in = input.size();
  const uint8_t *next_in = reinterpret_cast<const uint8_t *>(input.data());
  size_t available_out = output.size();
  uint8_t *next_out = reinterpret_cast<uint8_t *>(output.data());

  BrotliEncoderCompressStream(
    state, BROTLI_OPERATION_PROCESS, &available_in, &next_in, &available_out, &next_out, nullptr);

  size_t remaining_out = available_out;
  BrotliEncoderCompressStream(
    state, BROTLI_OPERATION_FINISH, &available_in, &next_in, &remaining_out, &next_out, nullptr);

  output.resize(output.size() - remaining_out);
  BrotliEncoderDestroyInstance(state);

  return output;
}
#endif
