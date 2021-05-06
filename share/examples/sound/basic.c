#include "ossinit.h"

int
main()
{
  config_t config = {
    .device = "/dev/dsp",
    .channels = -1,
    .format = format,
    .frag = -1,
    .sample_rate = 48000,
    .sample_size = sizeof(sample_t),
    .buffer_info.fragments = -1,
    .mmap = 0,
  };

  /* Initialize device */
  oss_init(&config);

  /* Allocate input and output buffers so that their size match frag_size */
  int bytes = config.buffer_info.bytes;
  int8_t ibuf[bytes];
  int8_t obuf[bytes];
  sample_t *channels = (sample_t *)malloc(bytes);
  printf(
    "bytes: %d, fragments: %d, fragsize: %d, fragstotal: %d, samples: %d\n",
    bytes,
    config.buffer_info.fragments,
    config.buffer_info.fragsize,
    config.buffer_info.fragstotal,
    config.sample_count
  );

  /* Minimal engine: read input and copy it to the output */
  while(1)
  {
    read(config.fd, ibuf, bytes);
    oss_split(&config, (sample_t *)ibuf, channels);
    /* All processing will happen here */
    oss_merge(&config, channels, (sample_t *)obuf);
    write(config.fd, obuf, bytes);
  }

  /* Cleanup */
  free(channels);
  close(config.fd);
  return 0;
}
