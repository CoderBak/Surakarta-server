#ifndef AGENT_AI_H
#define AGENT_AI_H

#include <memory>
#include "agent_base.h"

class AgentAI : public AgentBase {
public:
    using AgentBase::AgentBase;
    Move CalculateMove() override;
};

#endif //AGENT_AI_H
