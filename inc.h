#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <immintrin.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "GL/glut.h"
#include "GL/glu.h"
#include "pthread.h"

typedef unsigned long long u64;
typedef pthread_mutex_t mtex;

#include "fractalFloatingPoint.h"
#include "misc.h"
#include "fractalCalc.h"
#include "fractalIntrin.h"
#include "fractalThread.h"