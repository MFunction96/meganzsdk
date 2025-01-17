/**
 * @file mega/user.h
 * @brief Class for manipulating user / contact data
 *
 * (c) 2013-2014 by Mega Limited, Auckland, New Zealand
 *
 * This file is part of the MEGA SDK - Client Access Engine.
 *
 * Applications using the MEGA API must present a valid application key
 * and comply with the the rules set forth in the Terms of Service.
 *
 * The MEGA SDK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @copyright Simplified (2-clause) BSD License.
 *
 * You should have received a copy of the license along with this
 * program.
 */

#ifndef MEGA_USER_H
#define MEGA_USER_H 1

#include "attrmap.h"

namespace mega {
// user/contact
struct MEGA_API User : public Cachable
{
    // user handle
    handle userhandle;

    // string identifier for API requests (either e-mail address or ASCII user
    // handle)
    string uid;

    // e-mail address
    string email;

    // visibility status
    visibility_t show;

    // shares by this user
    handle_set sharing;

    // contact establishment timestamp
    m_time_t ctime;

    struct
    {
        bool keyring : 1;   // private keys
        bool authring : 1;  // authentication information of the contact
        bool lstint : 1;    // last interaction with the contact
        bool puEd255 : 1;   // public key for Ed25519
        bool puCu255 : 1;   // public key for Cu25519
        bool sigPubk : 1;   // signature for RSA public key
        bool sigCu255 : 1;  // signature for Cu255199 public key
        bool avatar : 1;    // avatar image
        bool firstname : 1;
        bool lastname : 1;
        bool country : 1;
        bool birthday : 1;      // wraps status of birthday, birthmonth, birthyear
        bool email : 1;
        bool language : 1;      // preferred language code
        bool pwdReminder : 1;   // password-reminder-dialog information
        bool disableVersions : 1;   // disable fileversioning
        bool contactLinkVerification : 1; // Verify contact requests with contact links
        bool richPreviews : 1;  // enable messages with rich previews
        bool lastPsa : 1;
        bool rubbishTime : 1;   // days to keep nodes in rubbish bin before auto clean
        bool storageState : 1;  // state of the storage (0 = green, 1 = orange, 2 = red)
        bool geolocation : 1;   // enable send geolocations
        bool cameraUploadsFolder : 1;   // target folder for Camera Uploads
        bool myChatFilesFolder : 1;   // target folder for my chat files
        bool pushSettings : 1;  // push notification settings
    } changed;

    // user's public key
    AsymmCipher pubk;
    struct
    {
        bool pubkrequested : 1;
        bool isTemporary : 1;
    };

    // actions to take after arrival of the public key
    deque<class PubKeyAction*> pkrs;

private:
    // persistent attributes (keyring, firstname...)
    userattr_map attrs;

    // version of each attribute
    userattr_map attrsv;

    // source tag
    int tag;

public:
    void set(visibility_t, m_time_t);

    bool serialize(string*);
    static User* unserialize(class MegaClient *, string*);

    // attribute methods: set/get/invalidate...
    void setattr(attr_t at, string *av, string *v);
    const string *getattr(attr_t at);
    const string *getattrversion(attr_t at);
    void invalidateattr(attr_t at);
    bool isattrvalid(attr_t at);
    void removeattr(attr_t at);

    static string attr2string(attr_t at);
    static string attr2longname(attr_t at);
    static attr_t string2attr(const char *name);
    static int needversioning(attr_t at);
    static char scope(attr_t at);

    enum {
        PWD_LAST_SUCCESS = 0x01,
        PWD_LAST_SKIPPED = 0x02,
        PWD_MK_EXPORTED = 0x04,
        PWD_DONT_SHOW = 0x08,
        PWD_LAST_LOGIN = 0x10
    };

    static const int PWD_SHOW_AFTER_ACCOUNT_AGE = 7 * 24 * 60 * 60;
    static const int PWD_SHOW_AFTER_LASTSUCCESS = 3 * 30 * 24 * 60 * 60;
    static const int PWD_SHOW_AFTER_LASTLOGIN = 14 * 24 * 60 * 60;
    static const int PWD_SHOW_AFTER_LASTSKIP = 3 * 30 * 24 * 60 * 60;
    static const int PWD_SHOW_AFTER_LASTSKIP_LOGOUT = 1 * 30 * 24 * 60 * 60;

    static bool mergePwdReminderData(int numDetails, const char *data, unsigned int size, string *newValue);
    static m_time_t getPwdReminderData(int numDetail, const char *data, unsigned int size);

    bool setChanged(attr_t at);

    void setTag(int tag);
    int getTag();
    void resetTag();

    User(const char* = NULL);
};
} // namespace

#endif
