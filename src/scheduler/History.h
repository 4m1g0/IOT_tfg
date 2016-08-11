#include <Arduino.h>
#include <ArduinoJson.h>
#include "../Serializable.h"

template <class T>
class Record : Serializable
{
public:
  void fromJson(JsonObject& json);
  JsonObject& toJson();
  long timestamp;
  T value;
};

template <class T>
class History : Serializable
{
public:
  void fromJson(String json);
  String toJson();
  void addValue(T value);

protected:
  static const int N = 60*24;
  Record<T> _records[N];
  int _last;
  int _next(int n);
};
