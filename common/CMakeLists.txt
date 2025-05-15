target_sources(taller_common
    PRIVATE
    # .cpp files
        catedra/liberror.cpp
        catedra/resolvererror.cpp
        catedra/resolver.cpp
        catedra/socket.cpp
        catedra/queue.cpp
        catedra/thread.cpp
        protocol.cpp
    PUBLIC
    # .h files
        catedra/liberror.h
        catedra/resolvererror.h
        catedra/resolver.h
        catedra/socket.h
        catedra/queue.h
        catedra/thread.h
        dto/game_state_update.h
        dto/login_request.h
        dto/player_action.h
        action.h
        message_type.h
        message.h
        protocol.h
    )
