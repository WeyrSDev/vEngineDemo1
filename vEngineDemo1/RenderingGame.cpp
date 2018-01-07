#include "RenderingGame.h"
#include <sstream>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "vException.h"
#include "vKeyboard.h"
#include "vMouse.h"
#include "vFpsCounter.h"
#include "vUtility.h"

namespace Rendering
{
    const XMVECTORF32 RenderingGame::BackgroundColor = { 0.392f, 0.584f, 0.929f, 1.0f };

    RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
        :  Engine(instance, windowClass, windowTitle, showCommand),
           mFpsComponent(nullptr),
           mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr),
           mSpriteBatch(nullptr), mSpriteFont(nullptr), mMouseTextPosition(0.0f, 20.0f)
    {
        mDepthStencilBufferEnabled = true;
        mMultiSamplingEnabled = true;
    }

    RenderingGame::~RenderingGame()
    {
    }

	void RenderingGame::Initialize()
	{
		if (FAILED(DirectInput8Create(mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDirectInput, nullptr)))
		{
			throw Exception("DirectInput8Create() failed");
		}

		mKeyboard = new Keyboard(*this, mDirectInput);
		mComponents.push_back(mKeyboard);
		mServices.AddService(Keyboard::TypeIdClass(), mKeyboard);
    
		mMouse = new Mouse(*this, mDirectInput);
		mComponents.push_back(mMouse);
		mServices.AddService(Mouse::TypeIdClass(), mMouse);
    
		mFpsComponent = new FpsCounter(*this);
		mComponents.push_back(mFpsComponent);

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		mSpriteBatch = new SpriteBatch(mDirect3DDeviceContext);
		mSpriteFont = new SpriteFont(mDirect3DDevice, L"Content\\Fonts\\Arial_14_Regular.spritefont");

		Engine::Initialize();
	}

    void RenderingGame::Shutdown()
    {
        DeleteObject(mKeyboard);
        DeleteObject(mMouse);
        DeleteObject(mFpsComponent);
        DeleteObject(mSpriteFont);
        DeleteObject(mSpriteBatch);

        ReleaseObject(mDirectInput);

        Engine::Shutdown();
    }

    void RenderingGame::Update(const Time &gameTime)
    {
        if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
        {
            Exit();
        }

        Engine::Update(gameTime);
    }

    void RenderingGame::Draw(const Time &gameTime)
    {
        mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&BackgroundColor));
        mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        Engine::Draw(gameTime);

        mSpriteBatch->Begin();
            
        std::wostringstream mouseLabel;
        mouseLabel << L"Mouse Position: " << mMouse->X() << ", " << mMouse->Y() << "  Mouse Wheel: " << mMouse->Wheel();
        mSpriteFont->DrawString(mSpriteBatch, mouseLabel.str().c_str(), mMouseTextPosition);

        mSpriteBatch->End();
        
        HRESULT hr = mSwapChain->Present(0, 0);
        if (FAILED(hr))
        {
            throw Exception("IDXGISwapChain::Present() failed.", hr);
        }
    }
}