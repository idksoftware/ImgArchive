/*
 * ImageLock.h
 *
 *  Created on: Sep 11, 2014
 *      Author: wzw7yn
 */

#ifndef IMAGELOCK_H_
#define IMAGELOCK_H_

namespace simplearchive {

class ImageLock {
public:
	ImageLock();
	virtual ~ImageLock();
	static bool isLocked(const char *filename);
	/// The Image file locked on checkout.
	/// returns false if locked already.
	static bool lock(const char *filename);
	/// The Image file locked on checkin.
	/// returns false if unlocked already.
	static bool unlock(const char *filename);

};

} /* namespace simplearchive */
#endif /* IMAGELOCK_H_ */
