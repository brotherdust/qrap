 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : Config.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Thane Thomson (roeland.frans@gmail.com)
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Ag just a multi-purpose header
 *
**************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; See the GNU General Public License for      *
 *   more details                                                          *
 *                                                                         *
 ************************************************************************* */

#ifndef Qrap_Config_h
#define Qrap_Config_h

// comment out to define client edition compile
//#define QRAP_SERVER_DAEMON
#define QRAP_SERVER_EDITION

#define QRAP_ALERT_DEFAULT_CALLBACK Qrap::ConsoleAlert
#define QRAP_DEBUG
#define ANTENNASCALE 60.0

#include <iostream>
#include <string.h>
#include <map>
#include <vector>

#include <pqxx/pqxx>
#include <pqxx/transaction>
#include <pqxx/transactor>
#include "doublefann.h"
#include "fann_cpp.h"
#include "Types.h"
#include "cAlert.h"
#include "cBase.h"
#include "cSettings.h"
#include "cDatabase.h"

#endif
