/***************************************************************************
 *   Copyright (C) 2007 by Sirrix AG security technologies
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef _TPMMANAGER_H_
#define _TPMMANAGER_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kmainwindow.h>

/**
 * @short Application Main Window
 * @author Anoosheh Zaerin <zaerin@sirrix.com> and Christian Stueble <stueble@sirrix.com>
 * @version 0.3
 */
class tpmmanager : public KMainWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    tpmmanager();

    /**
     * Default Destructor
     */
    virtual ~tpmmanager();
};

#endif // _TPMMANAGER_H_
