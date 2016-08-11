#include <Arduino.h>
#include "../Serializable.h"

enum NodeStatus { ON, OFF };
enum NodeType
{
  SCHEDULABLE, // Can be started whenever is apropiate, but once is running it must keep on until finished
  SCHEDULABLE_STOPABLE, // Can be started and stoped when needed
  STOPABLE, // Can not be scheduled but, if running, it can be stoped if needed
  REAL_TIME // Can not be scheduled nor stoped
};

class NodeInfo : Serializable
{
public:
  NodeStatus status;
  NodeType type;
  std::list<int> lista;
protected:

};
