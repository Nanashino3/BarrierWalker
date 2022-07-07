#ifndef SOUND_CSOUNDMANAGER_H_
#define SOUND_CSOUNDMANAGER_H_

namespace Sound
{
class CSoundManager
{
public:
	static void CreateInstance();
	static CSoundManager* GetInstance();
	static void DestroyInstance();

	void PlaySE(unsigned int);
	bool IsPlaySE(unsigned int);
	void StopSE(unsigned int);

	void PlayBGM(unsigned int);
	bool IsPlayBGM(unsigned int);
	void StopBGM(unsigned int);

	void AllStopSound();
private:
	CSoundManager();
	~CSoundManager(){}
	CSoundManager(const CSoundManager&){}
	CSoundManager& operator=(const CSoundManager&){}

private:
	static CSoundManager* s_myInstance;
};

} // namespace Sound

#endif // #ifndef SOUND_CSOUNDMANAGER_H_