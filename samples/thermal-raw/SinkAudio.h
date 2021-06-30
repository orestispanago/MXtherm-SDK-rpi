///////
     //
     //  MOBOTIX EventStream Client SDK 
     //
     //  This software is licensed under the BSD license,
     //  http://www.opensource.org/licenses/bsd-license.php
     //
     //  Copyright (c) 2005 - 2016, MOBOTIX AG
     //  All rights reserved.
     //
     //  Redistribution and use in source and binary forms, with or without
     //  modification, are permitted provided that the following conditions are
     //  met:
     //
     //  - Redistributions of source code must retain the above copyright
     //    notice, this list of conditions and the following disclaimer.
     //
     //  - Redistributions in binary form must reproduce the above copyright
     //    notice, this list of conditions and the following disclaimer in the
     //    documentation and/or other materials provided with the distribution.
     //
     //  - Neither the name of MOBOTIX AG nor the names of its contributors may
     //    be used to endorse or promote products derived from this software
     //    without specific prior written permission.
     //
     //  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     //  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     //  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
     //  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
     //  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
     //  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
     //  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
     //  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
     //  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
     //  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
     //  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
     //
///////

#ifndef SAMPLE_SINKAUDIO_H_
#define SAMPLE_SINKAUDIO_H_

#if defined (_MSC_VER)
#include <Windows.h>
#endif

#include "MxPEG_SinkAudio.hpp"
#include "MxPEG_Defines.hpp"

#include <stdio.h>
#include <memory>


using namespace ie::MxPEG;
/*
 * Sample audio sink, writes the raw audio stream to the specified file
 */
class SinkAudio : public MxPEG_SinkAudio
{
public:
   SinkAudio ();
   virtual
   ~SinkAudio ();

   virtual
   MxPEG_ReturnCode closeAudioDevice();

   virtual
   MxPEG_ReturnCode initAudioDevice();

protected:

   virtual
   MxPEG_ReturnCode doConsumeAudio(MxPEG_Audio::unique_ptr_t buffer);

   virtual
   MxPEG_AudioFormat * doGetTargetFormat();

};

#endif /* SAMPLE_SINKAUDIO_H_ */
