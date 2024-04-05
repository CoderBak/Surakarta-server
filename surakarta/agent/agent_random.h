#ifndef AGENT_RANDOM_H
#define AGENT_RANDOM_H

#include <memory>
#include "agent_base.h"

class AgentRandom : public AgentBase {
public:
    using AgentBase::AgentBase;

    Move CalculateMove() override;
};

#endif //AGENT_RANDOM_H
