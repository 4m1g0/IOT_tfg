/*
 History.cpp - Class to store history values
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

#include <Arduino.h>
#include "History.h"
#include "../Clock.h"
#include <ArduinoJson.h>

template <class T>
Record<T>::Record()
: timestamp(0)
, value(0)
{ }

template <class T>
Record<T>::Record(unsigned long timestamp, T value)
: timestamp(timestamp)
, value(value)
{ }

template <class T>
JsonObject& Record<T>::toJson()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root.set<long>("t", timestamp);
  root.set<T>("v", value);
  return root;
}

template <class T>
void Record<T>::fromJson(JsonObject& json)
{
  timestamp = json.get<unsigned long>("t");
  value = json.get<float>("v");
}

History::History()
: _last(0)
{
  for (int i = 0; i < N; i++)
    _records[i] = 0;
}

void History::addValue(unsigned short value)
{
  _last = _next(_last);
  _records[_last] = value;
}


void History::toJson(JsonArray& json)
{
  for (int i = _next(_last); i != _last; i = _next(i))
    json.add(_records[i]);

  json.add<unsigned short>(_records[_last]);
}

void History::fromJson(JsonArray& json)
{
  int i,j;
  for (i = json.size() > N ? json.size() - N : 0, j = 0;
       i < json.size();
       i++, j++)
  {
    _records[j] = json.get(i);
  }

  _last = j;
}

int History::_next(int n)
{
  return ++n%N;
}

// This tells the compiler which types are going to be used so that it can
// compile a instance of this class for that especific type.
// Thanks for this to Aaron McDaid (http://stackoverflow.com/questions/8752837)
//template class History<float>;
template class Record<float>;
