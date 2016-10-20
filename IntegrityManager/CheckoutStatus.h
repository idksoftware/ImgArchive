#pragma once

namespace simplearchive {

	class CheckoutStatus
	{
		static std::string m_shadow;
		static std::string m_workspace;

	public:
		CheckoutStatus();
		~CheckoutStatus();

		static bool Init(const char *shadow, const char *workspace);

		bool showCheckedOut(const char *path);
		bool showUncheckedOutChanges(const char *path);
		bool checkout(const char *imagePath);
		bool checkin(const char *imagePath);
	};

};