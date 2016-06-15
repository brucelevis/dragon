ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(proto)
ADD_SUBDIRECTORY(super)
ADD_SUBDIRECTORY(record)
ADD_SUBDIRECTORY(session)
ADD_SUBDIRECTORY(scene)
ADD_SUBDIRECTORY(gate)

# Project dependencies
ADD_DEPENDENCIES(super core proto)
ADD_DEPENDENCIES(record core proto)
ADD_DEPENDENCIES(session core proto)
ADD_DEPENDENCIES(scene core proto)
ADD_DEPENDENCIES(gate core proto)