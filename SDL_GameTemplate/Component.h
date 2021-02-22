#pragma once
/// <summary>
/// Base class for each component 
/// </summary>
#include<iostream>

using namespace std;

class Component
{
public:
   virtual ~Component() = default;
   virtual void init() = 0;
   virtual void draw() = 0;
};