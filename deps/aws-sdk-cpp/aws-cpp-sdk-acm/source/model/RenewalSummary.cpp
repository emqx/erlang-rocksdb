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

#include <aws/acm/model/RenewalSummary.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <utility>

using namespace Aws::Utils::Json;
using namespace Aws::Utils;

namespace Aws
{
namespace ACM
{
namespace Model
{

RenewalSummary::RenewalSummary() : 
    m_renewalStatus(RenewalStatus::NOT_SET),
    m_renewalStatusHasBeenSet(false),
    m_domainValidationOptionsHasBeenSet(false),
    m_renewalStatusReason(FailureReason::NOT_SET),
    m_renewalStatusReasonHasBeenSet(false),
    m_updatedAtHasBeenSet(false)
{
}

RenewalSummary::RenewalSummary(JsonView jsonValue) : 
    m_renewalStatus(RenewalStatus::NOT_SET),
    m_renewalStatusHasBeenSet(false),
    m_domainValidationOptionsHasBeenSet(false),
    m_renewalStatusReason(FailureReason::NOT_SET),
    m_renewalStatusReasonHasBeenSet(false),
    m_updatedAtHasBeenSet(false)
{
  *this = jsonValue;
}

RenewalSummary& RenewalSummary::operator =(JsonView jsonValue)
{
  if(jsonValue.ValueExists("RenewalStatus"))
  {
    m_renewalStatus = RenewalStatusMapper::GetRenewalStatusForName(jsonValue.GetString("RenewalStatus"));

    m_renewalStatusHasBeenSet = true;
  }

  if(jsonValue.ValueExists("DomainValidationOptions"))
  {
    Array<JsonView> domainValidationOptionsJsonList = jsonValue.GetArray("DomainValidationOptions");
    for(unsigned domainValidationOptionsIndex = 0; domainValidationOptionsIndex < domainValidationOptionsJsonList.GetLength(); ++domainValidationOptionsIndex)
    {
      m_domainValidationOptions.push_back(domainValidationOptionsJsonList[domainValidationOptionsIndex].AsObject());
    }
    m_domainValidationOptionsHasBeenSet = true;
  }

  if(jsonValue.ValueExists("RenewalStatusReason"))
  {
    m_renewalStatusReason = FailureReasonMapper::GetFailureReasonForName(jsonValue.GetString("RenewalStatusReason"));

    m_renewalStatusReasonHasBeenSet = true;
  }

  if(jsonValue.ValueExists("UpdatedAt"))
  {
    m_updatedAt = jsonValue.GetDouble("UpdatedAt");

    m_updatedAtHasBeenSet = true;
  }

  return *this;
}

JsonValue RenewalSummary::Jsonize() const
{
  JsonValue payload;

  if(m_renewalStatusHasBeenSet)
  {
   payload.WithString("RenewalStatus", RenewalStatusMapper::GetNameForRenewalStatus(m_renewalStatus));
  }

  if(m_domainValidationOptionsHasBeenSet)
  {
   Array<JsonValue> domainValidationOptionsJsonList(m_domainValidationOptions.size());
   for(unsigned domainValidationOptionsIndex = 0; domainValidationOptionsIndex < domainValidationOptionsJsonList.GetLength(); ++domainValidationOptionsIndex)
   {
     domainValidationOptionsJsonList[domainValidationOptionsIndex].AsObject(m_domainValidationOptions[domainValidationOptionsIndex].Jsonize());
   }
   payload.WithArray("DomainValidationOptions", std::move(domainValidationOptionsJsonList));

  }

  if(m_renewalStatusReasonHasBeenSet)
  {
   payload.WithString("RenewalStatusReason", FailureReasonMapper::GetNameForFailureReason(m_renewalStatusReason));
  }

  if(m_updatedAtHasBeenSet)
  {
   payload.WithDouble("UpdatedAt", m_updatedAt.SecondsWithMSPrecision());
  }

  return payload;
}

} // namespace Model
} // namespace ACM
} // namespace Aws