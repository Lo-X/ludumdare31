#include <states/gamestate.h>

GameState::GameState(StateStack &stack, Context context) :
    State(stack, context),
    mWorld(*context.window, *context.textures, *context.fonts, *context.music, *context.sounds, *context.scripts, mPlayer)
{
    //context.music->play(Musics::MainTheme);
    context.music->setVolume(5.f);
    context.music->setSpeed(1.0f);
}


void GameState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    mWorld.draw();
}


bool GameState::update(sf::Time dt)
{
    // Mission status check
    mWorld.update(dt);

    if(!mWorld.hasPlayerAlive())
    {
        requestStackPush(States::GameOver);
        return false;
    }

    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);

    // If time, updte some scripted events

    return true;
}


bool GameState::handleEvent(const sf::Event &event)
{
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commands);

    // If ESC key is pressed, we leave
    if (event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Escape:
                requestStackPop();
                //requestStackPush(States::Title);
                break;

            default:break;
        }
    }

    return true;
}
