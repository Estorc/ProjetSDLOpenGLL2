#pragma once

/**
 * @file structs.h
 * @brief This file contains forward declarations of all structures used in the engine.
 * 
 * This file contains forward declarations of all structures used in the engine.
 * Forward declarations are used to declare a structure without defining its members.
 * This allows the structure to be used in other structures or functions without including its definition.
 * 
 * @note Forward declarations are useful when structures are used in multiple files, reducing the number of includes.
 * @note Forward declarations are also useful when structures reference each other, avoiding circular dependencies.
 */

struct MemoryCaches;
struct BufferCollection;
struct Queue;
struct Tree;
struct Input;
struct Settings;
struct Window;
struct Camera;
struct RenderTarget;
struct Script;
struct ClassManager;
struct HashTable;
struct DepthMap;