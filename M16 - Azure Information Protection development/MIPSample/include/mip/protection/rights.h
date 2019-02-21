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
 * @brief Defines functions describing rights
 * 
 * @file rights.h
 */

#ifndef API_MIP_PROTECTION_RIGHTS_H_
#define API_MIP_PROTECTION_RIGHTS_H_

#include <string>
#include <vector>

namespace mip {

namespace rights {
/**
 * @brief Gets string identifier for 'owner' right
 * 
 * @return String identifier for 'owner' right
 */
inline std::string Owner() { return "OWNER"; }

/**
 * @brief Gets string identifier for 'view' right
 * 
 * @return String identifier for 'view' right
 */
inline std::string View() { return "VIEW"; }

/**
 * @brief Gets string identifier for 'audited extract' right
 * 
 * @return String identifier for 'audited extract' right
 */
inline std::string AuditedExtract() { return "AUDITEDEXTRACT"; }

/**
 * @brief Gets string identifier for 'edit' right
 * 
 * @return String identifier for 'edit' right
 */
inline std::string Edit() { return "EDIT"; }

/**
 * @brief Gets string identifier for 'export' right
 * 
 * @return String identifier for 'export' right
 */
inline std::string Export() { return "EXPORT"; }

/**
 * @brief Gets string identifier for 'extract' right
 * 
 * @return String identifier for 'extract' right
 */
inline std::string Extract() { return "EXTRACT"; }

/**
 * @brief Gets string identifier for 'print' right
 * 
 * @return String identifier for 'print' right
 */
inline std::string Print() { return "PRINT"; }

/**
 * @brief Gets string identifier for 'comment' right
 * 
 * @return String identifier for 'comment' right
 */
inline std::string Comment() { return "COMMENT"; }

/**
 * @brief Gets string identifier for 'reply' right
 * 
 * @return String identifier for 'reply' right
 */
inline std::string Reply() { return "REPLY"; }

/**
 * @brief Gets string identifier for 'reply all' right
 * 
 * @return String identifier for 'reply all' right
 */
inline std::string ReplyAll() { return "REPLYALL"; }

/**
 * @brief Gets string identifier for 'forward' right
 * 
 * @return String identifier for 'forward' right
 */
inline std::string Forward() { return "FORWARD"; }

/**
 * @brief Gets a list of rights that apply to emails 
 * 
 * @return A list of rights that apply to emails
 */
inline std::vector<std::string> EmailRights() {
  return std::vector<std::string>{Extract(), Forward(), Owner(), Print(), Reply(), ReplyAll(), View()};
}

/**
 * @brief Gets a list of rights that apply to documents 
 * 
 * @return A list of rights that apply to documents
 */
inline std::vector<std::string> EditableDocumentRights() {
  return std::vector<std::string>{Comment(), Edit(), Extract(), Owner(), Print(), View()};
}

/**
 * @brief Gets a list of rights that apply in all scenarios
 * 
 * @return A list of rights that apply in all scenarios 
 */
inline std::vector<std::string> CommonRights() {
  return std::vector<std::string>{Owner(), View()};
}


} // namespace rights
} // namespace mip

#endif // API_MIP_PROTECTION_RIGHTS_H_
