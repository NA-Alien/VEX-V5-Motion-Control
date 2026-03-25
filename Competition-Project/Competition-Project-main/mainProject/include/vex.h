#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "v5.h"
#include "v5_vcs.h"
#include "systemHeaders//drive.h"
#include "systemHeaders//exponen.h"
#include "systemHeaders//mainControls.h"
#include "systemHeaders//odom.h"
#include"systemHeaders//PID.h"
#include "systemHeaders//util.h"


#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)