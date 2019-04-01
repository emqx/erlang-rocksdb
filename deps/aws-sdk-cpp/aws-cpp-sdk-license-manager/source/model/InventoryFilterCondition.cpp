﻿/*
* Copyright 2010-2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License").
* You may not use this file except in compliance with the License.
* A copy of the License is located at
*
*  http://aws.amazon.com/apache2.0
*
* or in the "license" file accompanying this file. This file is distributed
* on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
* express or implied. See the License for the specific language governing
* permissions and limitations under the License.
*/

#include <aws/license-manager/model/InventoryFilterCondition.h>
#include <aws/core/utils/HashingUtils.h>
#include <aws/core/Globals.h>
#include <aws/core/utils/EnumParseOverflowContainer.h>

using namespace Aws::Utils;


namespace Aws
{
  namespace LicenseManager
  {
    namespace Model
    {
      namespace InventoryFilterConditionMapper
      {

        static const int EQUALS_HASH = HashingUtils::HashString("EQUALS");
        static const int NOT_EQUALS_HASH = HashingUtils::HashString("NOT_EQUALS");
        static const int BEGINS_WITH_HASH = HashingUtils::HashString("BEGINS_WITH");
        static const int CONTAINS_HASH = HashingUtils::HashString("CONTAINS");


        InventoryFilterCondition GetInventoryFilterConditionForName(const Aws::String& name)
        {
          int hashCode = HashingUtils::HashString(name.c_str());
          if (hashCode == EQUALS_HASH)
          {
            return InventoryFilterCondition::EQUALS;
          }
          else if (hashCode == NOT_EQUALS_HASH)
          {
            return InventoryFilterCondition::NOT_EQUALS;
          }
          else if (hashCode == BEGINS_WITH_HASH)
          {
            return InventoryFilterCondition::BEGINS_WITH;
          }
          else if (hashCode == CONTAINS_HASH)
          {
            return InventoryFilterCondition::CONTAINS;
          }
          EnumParseOverflowContainer* overflowContainer = Aws::GetEnumOverflowContainer();
          if(overflowContainer)
          {
            overflowContainer->StoreOverflow(hashCode, name);
            return static_cast<InventoryFilterCondition>(hashCode);
          }

          return InventoryFilterCondition::NOT_SET;
        }

        Aws::String GetNameForInventoryFilterCondition(InventoryFilterCondition enumValue)
        {
          switch(enumValue)
          {
          case InventoryFilterCondition::EQUALS:
            return "EQUALS";
          case InventoryFilterCondition::NOT_EQUALS:
            return "NOT_EQUALS";
          case InventoryFilterCondition::BEGINS_WITH:
            return "BEGINS_WITH";
          case InventoryFilterCondition::CONTAINS:
            return "CONTAINS";
          default:
            EnumParseOverflowContainer* overflowContainer = Aws::GetEnumOverflowContainer();
            if(overflowContainer)
            {
              return overflowContainer->RetrieveOverflow(static_cast<int>(enumValue));
            }

            return {};
          }
        }

      } // namespace InventoryFilterConditionMapper
    } // namespace Model
  } // namespace LicenseManager
} // namespace Aws