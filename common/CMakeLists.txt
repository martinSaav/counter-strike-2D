target_sources(taller_common
    PRIVATE
    # .cpp files
        foo.cpp
        catedra/liberror.cpp
        catedra/resolvererror.cpp
        catedra/resolver.cpp
        catedra/socket.cpp
    PUBLIC
    # .h files
        foo.h
        catedra/liberror.h
        catedra/resolvererror.h
        catedra/resolver.h
        catedra/socket.h
        catedra/queue.h
        catedra/thread.h
    )
