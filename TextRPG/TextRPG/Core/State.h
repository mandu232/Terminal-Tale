#pragma once

class InputManager;
class Context;

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
    virtual void Render() = 0;

protected:
    Context& context;
};