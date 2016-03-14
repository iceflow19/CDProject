/* 
 * File:   OptionParser.h
 * Author: Thomas Royko
 *
 * Created on February 28, 2016, 11:29 PM
 */

#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "ProgramOptions.h"

ProgramOptions * parse(int argc, char** argv);

#endif /* OPTIONPARSER_H */

