#ifndef IPlayer_Included
#define IPlayer_Included

namespace contracts
{
	namespace video
	{
		class IPlayer
		{
		public:
			virtual ~IPlayer(){}

			virtual void try_kill()   = 0;
			virtual void try_stop()   = 0;
			virtual void try_start()  = 0;
			virtual void try_pause()  = 0;
			virtual void try_resume() = 0;
		};

		class IMediaPlayer : public IPlayer
		{
		public:
			virtual ~IMediaPlayer() {}
		};
		/*, IMediaSettings { }*/

		enum PlayerCommands
		{
			  None = 0
			, Kill
			, Stop
			, Connect
			, Start
			, Pause
			, Resume
		};
	}
}

#endif