#pragma once

class InputManager;
class Context;
class ConsoleBuffer;

class State
{
public:
    State(Context& context)
        : context(context) {
    }

    virtual ~State() = default;

    virtual void Enter() {}
    virtual void Exit() {}

    virtual void HandleInput(InputManager& input) = 0;
    virtual void Update() = 0;
    virtual void Render(ConsoleBuffer& buffer) = 0;

protected:
    Context& context;
};