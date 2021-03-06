/*
 * dpf-webui
 * Copyright (C) 2021 Luciano Iam <lucianoiam@protonmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "BrowserHandler.hpp"

// FIXME - clashes with CEF
//#include <syslog.h>

#include <iostream>

#include "include/cef_app.h"
#include "include/wrapper/cef_helpers.h"

USE_NAMESPACE_DISTRHO

CefRefPtr<CefBrowser> BrowserHandler::getBrowser(uintptr_t parentWindowId)
{
    return mBrowserMap[parentWindowId];
}

bool BrowserHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& target_url,
    const CefString& target_frame_name,
    CefLifeSpanHandler::WindowOpenDisposition target_disposition,
    bool user_gesture,
    const CefPopupFeatures& popupFeatures,
    CefWindowInfo& windowInfo,
    CefRefPtr<CefClient>& client,
    CefBrowserSettings& settings,
    CefRefPtr<CefDictionaryValue>& extra_info,
    bool* no_javascript_access)
{
    CEF_REQUIRE_UI_THREAD();
    
    //syslog(LOG_INFO, "%p BrowserHandler::OnBeforePopup()", this);

    // Disable popups, only the main browser is allowed
    return true;
}

void BrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    //syslog(LOG_INFO, "%p BrowserHandler::OnAfterCreated()", this);

    // Add to the map of existing browsers.
    mBrowserMap.emplace(mNextParentWindowId, browser);
}

bool BrowserHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    //syslog(LOG_INFO, "%p BrowserHandler::DoClose()", this);

    // Closing the main window requires special handling. See the DoClose()
    // documentation in the CEF header for a detailed destription of this
    // process.

    // Allow the close. For windowed browsers this will result in the OS close
    // event being sent.

    return false;
}

void BrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    //syslog(LOG_INFO, "%p BrowserHandler::OnBeforeClose()", this);

    // Remove from the map of existing browsers.
    BrowserMap::iterator bit = mBrowserMap.begin();
    for (; bit != mBrowserMap.end(); ++bit) {
        if (bit->second->IsSame(browser)) {
            mBrowserMap.erase(bit);
            break;
        }
    }
}

void BrowserHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    ErrorCode errorCode,
                                    const CefString& errorText,
                                    const CefString& failedUrl)
{
    CEF_REQUIRE_UI_THREAD();

    //syslog(LOG_INFO, "%p BrowserHandler::OnLoadError()", this);
}
