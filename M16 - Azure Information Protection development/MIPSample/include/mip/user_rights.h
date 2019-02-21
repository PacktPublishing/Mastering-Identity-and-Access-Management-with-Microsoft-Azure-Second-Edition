/*
 *
 * Copyright (c) Microsoft Corporation.
 * All rights reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
/**
 * @brief Defines UserRights class
 * 
 * @file user_rights.h
 */

#ifndef API_MIP_USER_RIGHTS_H_
#define API_MIP_USER_RIGHTS_H_

#include <string>
#include <vector>

namespace mip {

/**
 * @brief A group of users and the rights associated with them
 */
class UserRights {
public:
  /**
   * @brief UserRights constructor
   * 
   * @param users Group of users that share the same rights
   * @param rights Rights shared by group of users
   */
  UserRights(const std::vector<std::string>& users, const std::vector<std::string>& rights) : mUsers(users), mRights(rights) {}

  /**
   * @brief Gets users associated with a set of rights
   * 
   * @return Users associated with a set of rights
   */
  const std::vector<std::string>& Users() const { return mUsers; }

  /**
   * @brief Gets rights associated with a group of users
   * 
   * @return Rights associated with a group of users
   */
  const std::vector<std::string>& Rights() const { return mRights; }

private:
  std::vector<std::string> mUsers;
  std::vector<std::string> mRights;
};
} // namespace mip

#endif // API_MIP_USER_RIGHTS_H_
