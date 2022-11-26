
#include "Starship_Navigator.h"

Input::Input(void)
{
    running = 0;
    video = 0;
    mouse = {};
    controller_deadzone = 0;
}

void Input::Init(bool* _running, Video* _video)
{
    running = _running;
    video = _video;
    controller_deadzone = 3200; //TODO: set in config
}

void Input::HandleEvents(void)
{
    SDL_Event event;

    // keys: set was_down state
    for (auto i : keys)
    {
        if (i.second.is_down && !i.second.was_down) i.second.was_down = 1;
        else if (!i.second.is_down && i.second.was_down) i.second.was_down = 0;
    }

    // Mouse: set was_down states
    if (mouse.left.is_down && !mouse.left.was_down)
    {
        mouse.left.was_down = 1;
    }
    else if (!mouse.left.is_down && mouse.left.was_down)
    {
        mouse.left.was_down = 0;
    }

    if (mouse.right.is_down && !mouse.right.was_down)
    {
        mouse.right.was_down = 1;
    }
    else if (!mouse.right.is_down && mouse.right.was_down)
    {
        mouse.right.was_down = 0;
    }

    if (mouse.middle.is_down && !mouse.middle.was_down)
    {
        mouse.middle.was_down = 1;
    }
    else if (!mouse.middle.is_down && mouse.middle.was_down)
    {
        mouse.middle.was_down = 0;
    }

    if (mouse.x1.is_down && !mouse.x1.was_down)
    {
        mouse.x1.was_down = 1;
    }
    else if (!mouse.x1.is_down && mouse.x1.was_down)
    {
        mouse.x1.was_down = 0;
    }

    if (mouse.x2.is_down && !mouse.x2.was_down)
    {
        mouse.x2.was_down = 1;
    }
    else if (!mouse.x2.is_down && mouse.x2.was_down)
    {
        mouse.x2.was_down = 0;
    }

    mouse.wheel = MW_NONE;

    // Controllers State
    {
        // remove old controllers
        for (auto it = controllers.cbegin(); it != controllers.cend();)
        {
            if (!it->second.connected)
            {
                controllers.erase(it++);
            }
            else
            {
                ++it;
            }
        }

        for (auto it = controllers.begin(); it != controllers.end(); it++)
        {
            if (it->second.connected)
            {
                if (SDL_GameControllerGetAttached(it->second.handle) == SDL_FALSE)
                {
                    SDL_GameControllerClose(it->second.handle);
                    it->second = {};
                    SDL_Log("Controller[%d] Disconnected", it->first);
                    break;
                }

                // Get Axis
                it->second.left_x = SDL_GameControllerGetAxis(it->second.handle, SDL_CONTROLLER_AXIS_LEFTX);
                it->second.left_y = SDL_GameControllerGetAxis(it->second.handle, SDL_CONTROLLER_AXIS_LEFTY);
                it->second.right_x = SDL_GameControllerGetAxis(it->second.handle, SDL_CONTROLLER_AXIS_RIGHTX);
                it->second.right_y = SDL_GameControllerGetAxis(it->second.handle, SDL_CONTROLLER_AXIS_RIGHTY);
                it->second.left_trigger = SDL_GameControllerGetAxis(it->second.handle, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
                it->second.right_trigger = SDL_GameControllerGetAxis(it->second.handle, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

                // Set was_down States
                if (it->second.a.is_down && !it->second.a.was_down)
                {
                    it->second.a.was_down = 1;
                }
                else if (!it->second.a.is_down && it->second.a.was_down)
                {
                    it->second.a.was_down = 0;
                }

                if (it->second.b.is_down && !it->second.b.was_down)
                {
                    it->second.b.was_down = 1;
                }
                else if (!it->second.b.is_down && it->second.b.was_down)
                {
                    it->second.b.was_down = 0;
                }

                if (it->second.x.is_down && !it->second.x.was_down)
                {
                    it->second.x.was_down = 1;
                }
                else if (!it->second.x.is_down && it->second.x.was_down)
                {
                    it->second.x.was_down = 0;
                }

                if (it->second.y.is_down && !it->second.y.was_down)
                {
                    it->second.y.was_down = 1;
                }
                else if (!it->second.y.is_down && it->second.y.was_down)
                {
                    it->second.y.was_down = 0;
                }

                if (it->second.back.is_down && !it->second.back.was_down)
                {
                    it->second.back.was_down = 1;
                }
                else if (!it->second.back.is_down && it->second.back.was_down)
                {
                    it->second.back.was_down = 0;
                }

                if (it->second.guide.is_down && !it->second.guide.was_down)
                {
                    it->second.guide.was_down = 1;
                }
                else if (!it->second.guide.is_down && it->second.guide.was_down)
                {
                    it->second.guide.was_down = 0;
                }

                if (it->second.start.is_down && !it->second.start.was_down)
                {
                    it->second.start.was_down = 1;
                }
                else if (!it->second.start.is_down && it->second.start.was_down)
                {
                    it->second.start.was_down = 0;
                }

                if (it->second.left_stick.is_down && !it->second.left_stick.was_down)
                {
                    it->second.left_stick.was_down = 1;
                }
                else if (!it->second.left_stick.is_down && it->second.left_stick.was_down)
                {
                    it->second.left_stick.was_down = 0;
                }

                if (it->second.right_stick.is_down && !it->second.right_stick.was_down)
                {
                    it->second.right_stick.was_down = 1;
                }
                else if (!it->second.right_stick.is_down && it->second.right_stick.was_down)
                {
                    it->second.right_stick.was_down = 0;
                }

                if (it->second.left_shoulder.is_down && !it->second.left_shoulder.was_down)
                {
                    it->second.left_shoulder.was_down = 1;
                }
                else if (!it->second.left_shoulder.is_down && it->second.left_shoulder.was_down)
                {
                    it->second.left_shoulder.was_down = 0;
                }

                if (it->second.right_shoulder.is_down && !it->second.right_shoulder.was_down)
                {
                    it->second.right_shoulder.was_down = 1;
                }
                else if (!it->second.right_shoulder.is_down && it->second.right_shoulder.was_down)
                {
                    it->second.right_shoulder.was_down = 0;
                }

                if (it->second.dpad_up.is_down && !it->second.dpad_up.was_down)
                {
                    it->second.dpad_up.was_down = 1;
                }
                else if (!it->second.dpad_up.is_down && it->second.dpad_up.was_down)
                {
                    it->second.dpad_up.was_down = 0;
                }

                if (it->second.dpad_down.is_down && !it->second.dpad_down.was_down)
                {
                    it->second.dpad_down.was_down = 1;
                }
                else if (!it->second.dpad_down.is_down && it->second.dpad_down.was_down)
                {
                    it->second.dpad_down.was_down = 0;
                }

                if (it->second.dpad_left.is_down && !it->second.dpad_left.was_down)
                {
                    it->second.dpad_left.was_down = 1;
                }
                else if (!it->second.dpad_left.is_down && it->second.dpad_left.was_down)
                {
                    it->second.dpad_left.was_down = 0;
                }

                if (it->second.dpad_right.is_down && !it->second.dpad_right.was_down)
                {
                    it->second.dpad_right.was_down = 1;
                }
                else if (!it->second.dpad_right.is_down && it->second.dpad_right.was_down)
                {
                    it->second.dpad_right.was_down = 0;
                }

                if (it->second.misc1.is_down && !it->second.misc1.was_down)
                {
                    it->second.misc1.was_down = 1;
                }
                else if (!it->second.misc1.is_down && it->second.misc1.was_down)
                {
                    it->second.misc1.was_down = 0;
                }

                if (it->second.paddle1.is_down && !it->second.paddle1.was_down)
                {
                    it->second.paddle1.was_down = 1;
                }
                else if (!it->second.paddle1.is_down && it->second.paddle1.was_down)
                {
                    it->second.paddle1.was_down = 0;
                }

                if (it->second.paddle2.is_down && !it->second.paddle2.was_down)
                {
                    it->second.paddle2.was_down = 1;
                }
                else if (!it->second.paddle2.is_down && it->second.paddle2.was_down)
                {
                    it->second.paddle2.was_down = 0;
                }

                if (it->second.paddle3.is_down && !it->second.paddle3.was_down)
                {
                    it->second.paddle3.was_down = 1;
                }
                else if (!it->second.paddle3.is_down && it->second.paddle3.was_down)
                {
                    it->second.paddle3.was_down = 0;
                }

                if (it->second.paddle4.is_down && !it->second.paddle4.was_down)
                {
                    it->second.paddle4.was_down = 1;
                }
                else if (!it->second.paddle4.is_down && it->second.paddle4.was_down)
                {
                    it->second.paddle4.was_down = 0;
                }

                if (it->second.touchpad.is_down && !it->second.touchpad.was_down)
                {
                    it->second.touchpad.was_down = 1;
                }
                else if (!it->second.touchpad.is_down && it->second.touchpad.was_down)
                {
                    it->second.touchpad.was_down = 0;
                }

                // Set is_down States
                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_A))
                {
                    if (!it->second.a.is_down)
                    {
                        it->second.a.is_down = 1;
                    }
                }
                else
                {
                    it->second.a.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_B))
                {
                    if (!it->second.b.is_down)
                    {
                        it->second.b.is_down = 1;
                    }
                }
                else
                {
                    it->second.b.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_X))
                {
                    if (!it->second.x.is_down)
                    {
                        it->second.x.is_down = 1;
                    }
                }
                else
                {
                    it->second.x.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_Y))
                {
                    if (!it->second.y.is_down)
                    {
                        it->second.y.is_down = 1;
                    }
                }
                else
                {
                    it->second.y.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_BACK))
                {
                    if (!it->second.back.is_down)
                    {
                        it->second.back.is_down = 1;
                    }
                }
                else
                {
                    it->second.back.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_GUIDE))
                {
                    if (!it->second.guide.is_down)
                    {
                        it->second.guide.is_down = 1;
                    }
                }
                else
                {
                    it->second.guide.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_START))
                {
                    if (!it->second.start.is_down)
                    {
                        it->second.start.is_down = 1;
                    }
                }
                else
                {
                    it->second.start.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_LEFTSTICK))
                {
                    if (!it->second.left_stick.is_down)
                    {
                        it->second.left_stick.is_down = 1;
                    }
                }
                else
                {
                    it->second.left_stick.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_RIGHTSTICK))
                {
                    if (!it->second.right_stick.is_down)
                    {
                        it->second.right_stick.is_down = 1;
                    }
                }
                else
                {
                    it->second.right_stick.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
                {
                    if (!it->second.left_shoulder.is_down)
                    {
                        it->second.left_shoulder.is_down = 1;
                    }
                }
                else
                {
                    it->second.left_shoulder.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
                {
                    if (!it->second.right_shoulder.is_down)
                    {
                        it->second.right_shoulder.is_down = 1;
                    }
                }
                else
                {
                    it->second.right_shoulder.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_DPAD_UP))
                {
                    if (!it->second.dpad_up.is_down)
                    {
                        it->second.dpad_up.is_down = 1;
                    }
                }
                else
                {
                    it->second.dpad_up.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
                {
                    if (!it->second.dpad_down.is_down)
                    {
                        it->second.dpad_down.is_down = 1;
                    }
                }
                else
                {
                    it->second.dpad_down.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
                {
                    if (!it->second.dpad_left.is_down)
                    {
                        it->second.dpad_left.is_down = 1;
                    }
                }
                else
                {
                    it->second.dpad_left.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
                {
                    if (!it->second.dpad_right.is_down)
                    {
                        it->second.dpad_right.is_down = 1;
                    }
                }
                else
                {
                    it->second.dpad_right.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_MISC1))
                {
                    if (!it->second.misc1.is_down)
                    {
                        it->second.misc1.is_down = 1;
                    }
                }
                else
                {
                    it->second.misc1.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_PADDLE1))
                {
                    if (!it->second.paddle1.is_down)
                    {
                        it->second.paddle1.is_down = 1;
                    }
                }
                else
                {
                    it->second.paddle1.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_PADDLE2))
                {
                    if (!it->second.paddle2.is_down)
                    {
                        it->second.paddle2.is_down = 1;
                    }
                }
                else
                {
                    it->second.paddle2.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_PADDLE3))
                {
                    if (!it->second.paddle3.is_down)
                    {
                        it->second.paddle3.is_down = 1;
                    }
                }
                else
                {
                    it->second.paddle3.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_PADDLE4))
                {
                    if (!it->second.paddle4.is_down)
                    {
                        it->second.paddle4.is_down = 1;
                    }
                }
                else
                {
                    it->second.paddle4.is_down = 0;
                }

                if (SDL_GameControllerGetButton(it->second.handle, SDL_CONTROLLER_BUTTON_TOUCHPAD))
                {
                    if (!it->second.touchpad.is_down)
                    {
                        it->second.touchpad.is_down = 1;
                    }
                }
                else
                {
                    it->second.touchpad.is_down = 0;
                }
            }
        }
    }

    if (SDL_NumJoysticks() > 0)
    {
        SDL_JoystickUpdate();
    }

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                if (running) *running = 0;
            } break;

            case SDL_WINDOWEVENT:
            {
                if (video)
                {
                    if (!video->windows.size())
                    {
                        if (running) *running = 0;
                    }

                    for (auto i : video->windows)
                    {
                        if (i.second->sdl_window_id == event.window.windowID)
                        {
                            switch (event.window.event)
                            {
                                case SDL_WINDOWEVENT_SHOWN:
                                {
                                    i.second->Resize();
                                } break;

                                case SDL_WINDOWEVENT_HIDDEN:
                                {
                                    i.second->Resize();
                                } break;

                                case SDL_WINDOWEVENT_EXPOSED:
                                {
                                    i.second->exposed = 1;
                                    i.second->minimized = 0;
                                    i.second->Resize();
                                } break;

                                case SDL_WINDOWEVENT_MOVED:
                                {
                                    i.second->Resize();
                                } break;
                                case SDL_WINDOWEVENT_SIZE_CHANGED:
                                {
                                    i.second->Resize();
                                } break;

                                case SDL_WINDOWEVENT_MINIMIZED:
                                {
                                    i.second->exposed = 0;
                                    i.second->minimized = 1;
                                    i.second->Resize();
                                } break;

                                case SDL_WINDOWEVENT_MAXIMIZED:
                                {
                                    i.second->maximized = 1;
                                    i.second->Resize();
                                } break;

                                case SDL_WINDOWEVENT_RESTORED:
                                {
                                    i.second->minimized = 0;
                                    i.second->maximized = 0;
                                    i.second->Resize();
                                } break;

                                case SDL_WINDOWEVENT_ENTER:
                                {
                                    i.second->mouse_focus = 1;
                                } break;

                                case SDL_WINDOWEVENT_LEAVE:
                                {
                                    i.second->mouse_focus = 0;
                                } break;

                                case SDL_WINDOWEVENT_FOCUS_GAINED:
                                {
                                    i.second->keyboard_focus = 1;
                                } break;

                                case SDL_WINDOWEVENT_FOCUS_LOST:
                                {
                                    i.second->keyboard_focus = 0;
                                } break;

                                case SDL_WINDOWEVENT_CLOSE:
                                {
                                    std::string id;
                                    for (auto i : video->windows)
                                    {
                                        if (event.window.windowID == i.second->sdl_window_id)
                                        {
                                            id = i.first;
                                            break;
                                        }
                                    }

                                    if (id.length()) video->DeleteWindow(id);

                                    if (id == "main") if (running) *running = 0;

                                } break;
                            }

                            break;
                        } // window ID
                    } // for(windows)
                } // if(video)

            } break; // case: window event

            case SDL_KEYDOWN:
            {
                keys[event.key.keysym.sym].is_down = 1;
                keys[event.key.keysym.sym].text_down = 1;
            } break;

            case SDL_KEYUP:
            {
                keys[event.key.keysym.sym].is_down = 0;
                keys[event.key.keysym.sym].text_down = 0;
            } break;

            //case SDL_TEXTEDITING:
            //case SDL_TEXTINPUT:
            //{
            //} break;

            case SDL_MOUSEWHEEL:
            {
                if (event.wheel.y > 0)
                {
                    mouse.wheel = MW_UP;
                }
                else if (event.wheel.y < 0)
                {
                    mouse.wheel = MW_DOWN;
                }
                else if (event.wheel.x < 0)
                {
                    mouse.wheel = MW_LEFT;
                }
                else if (event.wheel.x > 0)
                {
                    mouse.wheel = MW_RIGHT;
                }
            } break;

            case SDL_MOUSEMOTION:
            {
                if (video)
                {
                    for (auto i : video->windows)
                    {
                        if (i.second->sdl_window_id == event.window.windowID)
                        {
                            if (i.second->mouse_focus)
                            {
                                //TODO: separate mouse state by window and global (desktop pos vs window pos)
                                SDL_GetMouseState(&mouse.x, &mouse.y);
                            }

                            break;
                        }
                    }
                }
            } break;

            case SDL_MOUSEBUTTONDOWN:
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouse.left.is_down = 1;

                if (event.button.button == SDL_BUTTON_RIGHT)
                    mouse.right.is_down = 1;

                if (event.button.button == SDL_BUTTON_MIDDLE)
                    mouse.middle.is_down = 1;

                if (event.button.button == SDL_BUTTON_X1)
                    mouse.x1.is_down = 1;

                if (event.button.button == SDL_BUTTON_X2)
                    mouse.x2.is_down = 1;

            } break;

            case SDL_MOUSEBUTTONUP:
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouse.left.is_down = 0;

                if (event.button.button == SDL_BUTTON_RIGHT)
                    mouse.right.is_down = 0;

                if (event.button.button == SDL_BUTTON_MIDDLE)
                    mouse.middle.is_down = 0;

                if (event.button.button == SDL_BUTTON_X1)
                    mouse.x1.is_down = 0;

                if (event.button.button == SDL_BUTTON_X2)
                    mouse.x2.is_down = 0;
            } break;

            case SDL_JOYDEVICEADDED:
            {
                Controller new_controller = {};
                new_controller.handle = SDL_GameControllerOpen(event.cdevice.which);
                new_controller.id = event.cdevice.which;
                new_controller.connected = 1;
                controllers[event.cdevice.which] = new_controller;
                SDL_Log("Controller[%d] Connected", event.cdevice.which);

                // ensure no doubles
                for (auto it = controllers.begin(); it != controllers.end(); it++)
                {
                    for (auto it2 = controllers.begin(); it2 != controllers.end(); it2++)
                    {
                        if (it2->first != it->first && it->second.handle == it2->second.handle)
                        {
                            SDL_GameControllerClose(it2->second.handle);
                            it2->second = {};
                        }
                    }
                }

            } break;

            case SDL_JOYDEVICEREMOVED:
            {
                for (int i = 0; i < controllers.size(); i++)
                {
                    if (controllers[i].id == event.cdevice.which)
                    {
                        if (controllers[i].connected)
                        {
                            SDL_GameControllerClose(controllers[i].handle);
                            controllers[i] = {};
                            SDL_Log("Controller[%d] Disconnected", i);
                            break;
                        }
                    }
                }
            } break;

        } // switch event.type
    } // while poll event
}

bool Input::KeySingle(SDL_Keycode _key)
{
    if (keys.find(_key) == keys.end())
    {
        keys[_key] = {};
    }

    if (keys[_key].was_down)
    {
        return 0;
    }

    if (!keys[_key].was_down && keys[_key].is_down)
    {
        return 1;
    }

    return 0;
}

bool Input::KeyDown(SDL_Keycode _key)
{
    if (keys.find(_key) == keys.end())
    {
        keys[_key] = {};
    }

    return keys[_key].is_down;
}

bool Input::KeyRepeat(SDL_Keycode _key)
{
    if (keys.find(_key) == keys.end())
    {
        keys[_key] = {};
    }

    if (keys[_key].text_down)
    {
        keys[_key].text_down = 0;
        return 1;
    }

    return 0;
}

bool Input::KeyUp(SDL_Keycode _key)
{
    if (keys.find(_key) == keys.end())
    {
        keys[_key] = {};
    }

    if (!keys[_key].is_down && keys[_key].was_down)
    {
        return 1;
    }

    return 0;
}

bool Input::MouseSingle(Uint8 _button)
{
    if (_button == SDL_BUTTON_LEFT)
    {
        if (mouse.left.was_down)
        {
            return 0;
        }

        if (!mouse.left.was_down && mouse.left.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_BUTTON_RIGHT)
    {
        if (mouse.right.was_down)
        {
            return 0;
        }

        if (!mouse.right.was_down && mouse.right.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_BUTTON_MIDDLE)
    {
        if (mouse.middle.was_down)
        {
            return 0;
        }

        if (!mouse.middle.was_down && mouse.middle.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_BUTTON_X1)
    {
        if (mouse.x1.was_down)
        {
            return 0;
        }

        if (!mouse.x1.was_down && mouse.x1.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_BUTTON_X2)
    {
        if (mouse.x2.was_down)
        {
            return 0;
        }

        if (!mouse.x2.was_down && mouse.x2.is_down)
        {
            return 1;
        }
    }

    return 0;
}

bool Input::MouseDown(Uint8 _button)
{
    if (_button == SDL_BUTTON_LEFT)
    {
        return mouse.left.is_down;
    }

    else if (_button == SDL_BUTTON_RIGHT)
    {
        return mouse.right.is_down;
    }

    else if (_button == SDL_BUTTON_MIDDLE)
    {
        return mouse.middle.is_down;
    }

    else if (_button == SDL_BUTTON_X1)
    {
        return mouse.x1.is_down;
    }

    else if (_button == SDL_BUTTON_X2)
    {
        return mouse.x2.is_down;
    }

    return 0;
}

bool Input::MouseUp(Uint8 _button)
{
    if (_button == SDL_BUTTON_LEFT)
    {
        if (!mouse.left.is_down && mouse.left.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_BUTTON_RIGHT)
    {
        if (!mouse.right.is_down && mouse.right.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_BUTTON_MIDDLE)
    {
        if (!mouse.middle.is_down && mouse.middle.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_BUTTON_X1)
    {
        if (!mouse.x1.is_down && mouse.x1.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_BUTTON_X2)
    {
        if (!mouse.x2.is_down && mouse.x2.was_down)
        {
            return 1;
        }

        return 0;
    }

    return 0;
}

bool Input::MouseWheel(MOUSEWHEEL _direction)
{
    if (mouse.wheel == _direction) return 1;
    else return 0;
}

bool Input::ControllerBtnSingle(int _controller, SDL_GameControllerButton _button)
{
    if (controllers.find(_controller) == controllers.end()) return 0;
    if (!controllers[_controller].connected) return 0;

    if (_button == SDL_CONTROLLER_BUTTON_A)
    {
        if (controllers[_controller].a.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].a.was_down &&
            controllers[_controller].a.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_B)
    {
        if (controllers[_controller].b.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].b.was_down &&
            controllers[_controller].b.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_X)
    {
        if (controllers[_controller].x.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].x.was_down &&
            controllers[_controller].x.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_Y)
    {
        if (controllers[_controller].y.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].y.was_down &&
            controllers[_controller].y.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_BACK)
    {
        if (controllers[_controller].back.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].back.was_down &&
            controllers[_controller].back.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_GUIDE)
    {
        if (controllers[_controller].guide.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].guide.was_down &&
            controllers[_controller].guide.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_START)
    {
        if (controllers[_controller].start.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].start.was_down &&
            controllers[_controller].start.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
    {
        if (controllers[_controller].left_stick.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].left_stick.was_down &&
            controllers[_controller].left_stick.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
    {
        if (controllers[_controller].right_stick.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].right_stick.was_down &&
            controllers[_controller].right_stick.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
    {
        if (controllers[_controller].left_shoulder.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].left_shoulder.was_down &&
            controllers[_controller].left_shoulder.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
    {
        if (controllers[_controller].right_shoulder.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].right_shoulder.was_down &&
            controllers[_controller].right_shoulder.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_UP)
    {
        if (controllers[_controller].dpad_up.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].dpad_up.was_down &&
            controllers[_controller].dpad_up.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
    {
        if (controllers[_controller].dpad_down.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].dpad_down.was_down &&
            controllers[_controller].dpad_down.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
    {
        if (controllers[_controller].dpad_left.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].dpad_left.was_down &&
            controllers[_controller].dpad_left.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
    {
        if (controllers[_controller].dpad_right.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].dpad_right.was_down &&
            controllers[_controller].dpad_right.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_MISC1)
    {
        if (controllers[_controller].misc1.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].misc1.was_down &&
            controllers[_controller].misc1.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE1)
    {
        if (controllers[_controller].paddle1.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].paddle1.was_down &&
            controllers[_controller].paddle1.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE2)
    {
        if (controllers[_controller].paddle2.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].paddle2.was_down &&
            controllers[_controller].paddle2.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE3)
    {
        if (controllers[_controller].paddle3.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].paddle3.was_down &&
            controllers[_controller].paddle3.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE4)
    {
        if (controllers[_controller].paddle4.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].paddle4.was_down &&
            controllers[_controller].paddle4.is_down)
        {
            return 1;
        }
    }

    else if (_button == SDL_CONTROLLER_BUTTON_TOUCHPAD)
    {
        if (controllers[_controller].touchpad.was_down)
        {
            return 0;
        }

        if (!controllers[_controller].touchpad.was_down &&
            controllers[_controller].touchpad.is_down)
        {
            return 1;
        }
    }

    return 0;
}

bool Input::ControllerBtnDown(int _controller, SDL_GameControllerButton _button)
{
    if (controllers.find(_controller) == controllers.end()) return 0;
    if (!controllers[_controller].connected) return 0;

    if (_button == SDL_CONTROLLER_BUTTON_A)
    {
        return controllers[_controller].a.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_B)
    {
        return controllers[_controller].b.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_X)
    {
        return controllers[_controller].x.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_Y)
    {
        return controllers[_controller].y.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_BACK)
    {
        return controllers[_controller].back.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_GUIDE)
    {
        return controllers[_controller].guide.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_START)
    {
        return controllers[_controller].start.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
    {
        return controllers[_controller].left_stick.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
    {
        return controllers[_controller].right_stick.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
    {
        return controllers[_controller].left_shoulder.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
    {
        return controllers[_controller].right_shoulder.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_UP)
    {
        return controllers[_controller].dpad_up.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
    {
        return controllers[_controller].dpad_down.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
    {
        return controllers[_controller].dpad_left.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
    {
        return controllers[_controller].dpad_right.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_MISC1)
    {
        return controllers[_controller].misc1.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE1)
    {
        return controllers[_controller].paddle1.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE2)
    {
        return controllers[_controller].paddle2.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE3)
    {
        return controllers[_controller].paddle3.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE4)
    {
        return controllers[_controller].paddle4.is_down;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_TOUCHPAD)
    {
        return controllers[_controller].touchpad.is_down;
    }

    return 0;
}

bool Input::ControllerBtnUp(int _controller, SDL_GameControllerButton _button)
{
    if (controllers.find(_controller) == controllers.end()) return 0;
    if (!controllers[_controller].connected) return 0;

    if (_button == SDL_CONTROLLER_BUTTON_A)
    {
        if (!controllers[_controller].a.is_down &&
            controllers[_controller].a.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_B)
    {
        if (!controllers[_controller].b.is_down &&
            controllers[_controller].b.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_X)
    {
        if (!controllers[_controller].x.is_down &&
            controllers[_controller].x.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_Y)
    {
        if (!controllers[_controller].y.is_down &&
            controllers[_controller].y.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_BACK)
    {
        if (!controllers[_controller].back.is_down &&
            controllers[_controller].back.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_GUIDE)
    {
        if (!controllers[_controller].guide.is_down &&
            controllers[_controller].guide.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_START)
    {
        if (!controllers[_controller].start.is_down &&
            controllers[_controller].start.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
    {
        if (!controllers[_controller].left_stick.is_down &&
            controllers[_controller].left_stick.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
    {
        if (!controllers[_controller].right_stick.is_down &&
            controllers[_controller].right_stick.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
    {
        if (!controllers[_controller].left_shoulder.is_down &&
            controllers[_controller].left_shoulder.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
    {
        if (!controllers[_controller].right_shoulder.is_down &&
            controllers[_controller].right_shoulder.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_UP)
    {
        if (!controllers[_controller].dpad_up.is_down &&
            controllers[_controller].dpad_up.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
    {
        if (!controllers[_controller].dpad_down.is_down &&
            controllers[_controller].dpad_down.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
    {
        if (!controllers[_controller].dpad_left.is_down &&
            controllers[_controller].dpad_left.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
    {
        if (!controllers[_controller].dpad_right.is_down &&
            controllers[_controller].dpad_right.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_MISC1)
    {
        if (!controllers[_controller].misc1.is_down &&
            controllers[_controller].misc1.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE1)
    {
        if (!controllers[_controller].paddle1.is_down &&
            controllers[_controller].paddle1.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE2)
    {
        if (!controllers[_controller].paddle2.is_down &&
            controllers[_controller].paddle2.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE3)
    {
        if (!controllers[_controller].paddle3.is_down &&
            controllers[_controller].paddle3.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_PADDLE4)
    {
        if (!controllers[_controller].paddle4.is_down &&
            controllers[_controller].paddle4.was_down)
        {
            return 1;
        }

        return 0;
    }

    else if (_button == SDL_CONTROLLER_BUTTON_TOUCHPAD)
    {
        if (!controllers[_controller].touchpad.is_down &&
            controllers[_controller].touchpad.was_down)
        {
            return 1;
        }

        return 0;
    }

    return 0;
}

i16 Input::ControllerAxisRaw(int _controller, SDL_GameControllerAxis _axis)
{
    if (controllers.find(_controller) == controllers.end()) return 0;
    if (!controllers[_controller].connected) return 0;

    i16 Result = 0;

    if (_axis == SDL_CONTROLLER_AXIS_LEFTX)
        Result = controllers[_controller].left_x;
    if (_axis == SDL_CONTROLLER_AXIS_LEFTY)
        Result = controllers[_controller].left_y;
    if (_axis == SDL_CONTROLLER_AXIS_RIGHTX)
        Result = controllers[_controller].right_x;
    if (_axis == SDL_CONTROLLER_AXIS_RIGHTY)
        Result = controllers[_controller].right_y;
    if (_axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
        Result = controllers[_controller].left_trigger;
    if (_axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
        Result = controllers[_controller].right_trigger;

    return Result;
}

i16 Input::ControllerAxisDZ(int _controller, SDL_GameControllerAxis _axis)
{
    if (controllers.find(_controller) == controllers.end()) return 0;
    if (!controllers[_controller].connected) return 0;

    i16 Result = 0;

    if (_axis == SDL_CONTROLLER_AXIS_LEFTX)
        Result = controllers[_controller].left_x;
    if (_axis == SDL_CONTROLLER_AXIS_LEFTY)
        Result = controllers[_controller].left_y;
    if (_axis == SDL_CONTROLLER_AXIS_RIGHTX)
        Result = controllers[_controller].right_x;
    if (_axis == SDL_CONTROLLER_AXIS_RIGHTY)
        Result = controllers[_controller].right_y;
    if (_axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
        Result = controllers[_controller].left_trigger;
    if (_axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
        Result = controllers[_controller].right_trigger;

    if (Result <= -controller_deadzone) return Result;
    if (Result >= controller_deadzone) return Result;

    return 0;
}