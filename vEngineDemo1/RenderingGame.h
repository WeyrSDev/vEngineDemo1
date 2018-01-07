#pragma once

#include "vInclude.h"
#include "vEngine.h"

using namespace vEngine;

namespace DirectX
{
    class SpriteBatch;
    class SpriteFont;
}

namespace vEngine
{
	class FpsCounter;
	class Keyboard;
	class Mouse;
}

namespace Rendering
{
    class RenderingGame : public Engine
    {
    public:
        RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand);
        ~RenderingGame();

        virtual void Initialize() override;		
        virtual void Update(const Time& gameTime) override;
        virtual void Draw(const Time& gameTime) override;

	protected:
		virtual void Shutdown() override;

    private:
        static const XMVECTORF32 BackgroundColor;

		LPDIRECTINPUT8 mDirectInput;
		Keyboard* mKeyboard;
		Mouse* mMouse;
		FpsCounter* mFpsComponent;

		SpriteBatch* mSpriteBatch;
        SpriteFont* mSpriteFont;
        XMFLOAT2 mMouseTextPosition;
    };
}