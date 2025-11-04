#pragma once

namespace neu {
	class GUI {
	public:
		virtual void UpdateGui() = 0;

	public:
		bool guiActive = true;
	};
}