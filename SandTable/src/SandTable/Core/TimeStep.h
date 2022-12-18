#pragma once
namespace SandTable
{
	class TimeStep
	{
	public:
		TimeStep(float fTime = 0.f);
		const float GetSeconds() const;
		const float GetMilliSeconds()const;
		operator float()const;
		static float GetTime();
	private:
		float m_fTime;
	};
}


