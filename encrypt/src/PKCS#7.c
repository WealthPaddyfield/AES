#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// pad data (returns malloc'ed buffer)
unsigned char *pkcs7_pad(const unsigned char *data, size_t data_len,
                         size_t *out_len) {
  const size_t B = 16;
  size_t pad = B - (data_len % B);
  *out_len = data_len + pad;
  unsigned char *buf = malloc(*out_len);
  if (!buf)
    return NULL;
  memcpy(buf, data, data_len);
  memset(buf + data_len, (unsigned char)pad, pad);
  return buf;
}

// unpad in-place; returns new length or -1 on invalid padding
ssize_t pkcs7_unpad(unsigned char *data, size_t data_len) {
  if (data_len == 0)
    return -1;
  unsigned char pad = data[data_len - 1];
  if (pad == 0 || pad > 16)
    return -1;
  for (size_t i = 0; i < pad; ++i) {
    if (data[data_len - 1 - i] != pad)
      return -1;
  }
  return (ssize_t)(data_len - pad);
}
