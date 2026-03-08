#include <amzn_secure_boot.h>

const char *amzn_target_device_name(void)
{
  return "margo_r2";
}

int amzn_target_device_type(void)
{
  return AMZN_ENGINEERING_DEVICE;
}
