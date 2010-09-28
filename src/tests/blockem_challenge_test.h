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
/// @file  blockem_challenge_test.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 28-Sep-2010  Original development
/// @endhistory
///
// ============================================================================

#include "blockem_challenge.h"

/// @brief regression testing for the blockem challenge class
class BlockemChallengeTest
{
public:
    BlockemChallengeTest();
    virtual ~BlockemChallengeTest();

    /// launch the test!!
    void DoTest();
    
private:
    /// @return true if the XML could be loaded. False if an exception ocurred
    bool TryLoadXMLChallenge(const char* fileName);
    
    /// Loads and check a preloaded valid challenge
    void LoadAndCheckValidChallenge();
};