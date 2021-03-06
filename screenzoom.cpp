/****************************************************************************
**
** Copyright (C) 2021 Keith Kyzivat
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "screenzoom.h"

//! [0]
ScreenZoom::ScreenZoom()
{
    setWindowFlag(Qt::FramelessWindowHint, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAlignment(Qt::AlignCenter);

    setMinimumSize(100, 100);

    QShortcut *quitShortcut = new QShortcut(tr("Ctrl+Q"), this);
    connect(quitShortcut, &QShortcut::activated, qApp, &QCoreApplication::quit);

    shotTimer = new QTimer(this);
    connect(shotTimer, &QTimer::timeout, this, &ScreenZoom::processCursorPosition);
    shotTimer->start(16);
    resize(200, 200);
}
//! [0]

//! [1]
void ScreenZoom::resizeEvent(QResizeEvent * /* event */)
{
    QSize scaledSize = originalPixmap.size();
    scaledSize.scale(size(), Qt::KeepAspectRatio);
    if (scaledSize != pixmap(Qt::ReturnByValue).size())
        updateScreenshotLabel();
}
//! [1]

void ScreenZoom::processCursorPosition()
{
    QPoint pt = QCursor::pos();
    if (pt == lastCursorPos)
        return;

    QScreen* screenAtCursor = QGuiApplication::screenAt(pt);
    if (!screenAtCursor)
        return;

    // If cursor is on a different screen than the label is, move it.
    QScreen *screen = window()->windowHandle()->screen();
    if (screenAtCursor != screen)
        move(screenAtCursor->geometry().topLeft());

    QPoint ptScreenRelative = pt - screenAtCursor->geometry().topLeft();
    originalPixmap = screenAtCursor->grabWindow(0, ptScreenRelative.x(), ptScreenRelative.y(), m_size.width(), m_size.height());
    updateScreenshotLabel();
    lastCursorPos = pt;
}

//! [10]
void ScreenZoom::updateScreenshotLabel()
{
    if (originalPixmap.isNull())
        return;

    setPixmap(originalPixmap.scaled(size()*originalPixmap.devicePixelRatioF(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::FastTransformation));
}
//! [10]
