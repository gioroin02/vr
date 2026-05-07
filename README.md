# VR software modules

### Memory management

There are three main allocator types: arena, stack and pool; the arena allocator is used to handle temporary allocations that are confined in a specific function or cycle/operation, the stack allocator is used to create and destroy things that are confined inside a pair of create/destroy for a class, the pool allocator is instead used to handle trees, lists or omogeneous elements that can come and go in any order (e.g. the tasks for async operations).

For strings that need to be terminated exist String8/16/32, to manipulate their content exist String_View and String_Buffer.
