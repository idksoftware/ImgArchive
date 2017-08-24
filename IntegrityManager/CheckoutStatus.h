#pragma once

namespace simplearchive {
	/**
	 * CheckoutStatus is used to control the checkout status of any image in the archive.
	 * As the derivatives are placed in the same workspace location as the master image
	 * then master archive check disk files are used.
	 */
	class CheckoutRow;
	class CheckoutStatus
	{
		enum class Status {
			Ok,
			InvalidPath,
			NotCheckedout
		};

		static std::string m_Master;
		static std::string m_workspace;
		static std::string m_primaryIndex;

		Status m_LastError;
		bool add(CheckoutRow &checkoutRow, const char *relpath);
	public:
		CheckoutStatus();
		~CheckoutStatus();
		Status getLastError() {
			return Status::Ok;
		}
		static bool Init(const char *Master, const char *workspace, const char *primaryIndex);
		bool newImage(const char *imagePath);
		bool showCheckedOut(const char *path);
		bool showUncheckedOutChanges(const char *path);
		bool checkout(const char *imagePath);
		bool checkin(const char *imagePath);
		//bool uncheckout(const char *imagePath);
	};

};