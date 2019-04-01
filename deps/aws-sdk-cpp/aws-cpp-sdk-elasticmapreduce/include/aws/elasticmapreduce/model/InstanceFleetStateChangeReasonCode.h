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

#pragma once
#include <aws/elasticmapreduce/EMR_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSString.h>

namespace Aws
{
namespace EMR
{
namespace Model
{
  enum class InstanceFleetStateChangeReasonCode
  {
    NOT_SET,
    INTERNAL_ERROR,
    VALIDATION_ERROR,
    INSTANCE_FAILURE,
    CLUSTER_TERMINATED
  };

namespace InstanceFleetStateChangeReasonCodeMapper
{
AWS_EMR_API InstanceFleetStateChangeReasonCode GetInstanceFleetStateChangeReasonCodeForName(const Aws::String& name);

AWS_EMR_API Aws::String GetNameForInstanceFleetStateChangeReasonCode(InstanceFleetStateChangeReasonCode value);
} // namespace InstanceFleetStateChangeReasonCodeMapper
} // namespace Model
} // namespace EMR
} // namespace Aws