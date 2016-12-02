/*
 History.h - Class to store history values
 Copyright (c) 2016 Oscar Blanco.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef  HISTORY_H
#define HISTORY_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../Serializable.h"

template <class T>
class Record
{
  // We don't have enough memory to hold this object in memory
public:
  Record();
  Record(unsigned long timestamp, T value);
  void fromJson(JsonObject& json);
  JsonObject& toJson();
  unsigned long timestamp;
  T value;
};

class History : public Serializable<JsonArray&>
{
public:
  History();
  void fromJson(JsonArray& json);
  void toJson(JsonArray& json);
  void addValue(unsigned short value);

protected:
  static const short N = 31;
  unsigned short _records[N];
  int _last;
  int _next(int n);
};
#endif
