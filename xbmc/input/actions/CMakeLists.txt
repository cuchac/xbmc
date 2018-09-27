set(SOURCES Action.cpp
            ActionTranslator.cpp
)

set(HEADERS interfaces/IActionHandler.h
            Action.h
            ActionIDs.h
            ActionTranslator.h
)

core_add_library(input_actions)
