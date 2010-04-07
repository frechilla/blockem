// ============================================================================
// Copyright 2010 Faustino Frechilla
//
// This file is part of Blockem.
//
// Blockem is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// Blockem is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along
// with Blockem. If not, see http://www.gnu.org/licenses/.
//
/// @file singleton.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 7-Apr-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

/// @brief Inheriting from this class you can make your class a singleton
///        so it can only be instantiated once
template <class TClass>
class Singleton
{
public:
    static TClass& Instance()
    {
        if (m_instancePtr == NULL)
        {
            static TClass instance;
            m_instancePtr = &instance;
        }

        return *m_instancePtr;
    }

    ///@brief returns a pointer to the instance.
    /// If the singleton has not been initialised it will return NULL
    static TClass* GetPtr()
    {
#ifdef DEBUG
        assert(m_instancePtr != NULL);
#endif

        return m_instancePtr;
    }

protected:
    Singleton(){};
    virtual ~Singleton(){};

    static TClass *m_instancePtr;
};

template <class TClass> TClass* Singleton<TClass>::m_instancePtr = NULL;

#endif // _SINGLETON_H_
