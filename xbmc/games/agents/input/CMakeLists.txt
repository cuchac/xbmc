set(SOURCES GameAgentInput.cpp
            GameAgentJoystick.cpp
)

set(HEADERS GameAgentInput.h
            GameAgentJoystick.h
            IGameAgentJoystickHandler.h
)

core_add_library(games_agent_input)
