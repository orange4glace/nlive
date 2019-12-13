#ifndef NLIVE_PLATFORM_INCLUDE_H_
#define NLIVE_PLATFORM_INCLUDE_H_

namespace nlive {

// Action
class IMenuService;
class IMenu;

// Commands
class ICommandService;
class IContext;
class IContextKey;
class RawContextKey;
struct IContextKeyChangeEvent;
class IContextKeyService;

// Resource
class IResourceService;

// Service
class IService;
class ServiceLocator;

// Task
class Task;
class ITaskService;

// Theme
class IThemeService;

// Undo
class IUndoStack;
class UndoCommand;

// Encoder
class IEncodingService;

}

#endif