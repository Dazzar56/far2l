#pragma once
#include <memory>
#include "../Backend.h"
#include "../FSClipboardBackend.h"

struct ITTYXGlue
{
	typedef std::map<std::string, std::vector<unsigned char>> Type2Data;

	virtual ~ITTYXGlue() {}

	virtual DWORD GetModifiers() noexcept = 0;
	virtual bool SetClipboard(const Type2Data &t2d) noexcept = 0;
	virtual bool GetClipboard(const std::string &type, std::vector<unsigned char> &data) noexcept = 0;
	virtual bool ContainsClipboard(const std::string &type) noexcept = 0;
};

typedef std::shared_ptr<ITTYXGlue> ITTYXGluePtr;

// Starts far2l_ttyx.broker process and returns instance of glue that communicates to it
// NB: instance of glue must be destroyed by same thread as one that called StartTTYX in 
// order to correctly join broker process.
ITTYXGluePtr StartTTYX(const char *full_exe_path);

class TTYXClipboard : public IClipboardBackend
{
	ITTYXGluePtr _ttyx;
	std::unique_ptr<ITTYXGlue::Type2Data> _pending_set;

protected:
	virtual bool OnClipboardOpen();
	virtual void OnClipboardClose();
	virtual void OnClipboardEmpty();
	virtual bool OnClipboardIsFormatAvailable(UINT format);
	virtual void *OnClipboardSetData(UINT format, void *data);
	virtual void *OnClipboardGetData(UINT format);
	virtual UINT OnClipboardRegisterFormat(const wchar_t *lpszFormat);

public:
	TTYXClipboard(ITTYXGluePtr &ttyx);
	virtual ~TTYXClipboard();
};