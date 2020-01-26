#ifndef PACENGINE_H
#define PACENGINE_H

namespace ve {

	class PacEngine :
		public VEEngine
	{
	public:
		//Constructor
		PacEngine(bool debug = false) : VEEngine(debug) {};

		//Destructor
		~PacEngine() {};

		virtual void loadLevel(uint32_t numLevel = 1);
		virtual void registerEventListeners();
	};
}

#endif