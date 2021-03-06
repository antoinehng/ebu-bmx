/*
 * Copyright (C) 2011, British Broadcasting Corporation
 * All Rights Reserved.
 *
 * Author: Philip de Nier
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the British Broadcasting Corporation nor the names
 *       of its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __BMX_MXF_SEQUENCE_READER_H__
#define __BMX_MXF_SEQUENCE_READER_H__

#include <vector>

#include <bmx/mxf_reader/MXFReader.h>
#include <bmx/mxf_reader/MXFSequenceTrackReader.h>



namespace bmx
{


class MXFSequenceReader : public MXFReader
{
public:
    MXFSequenceReader();
    virtual ~MXFSequenceReader();

    void AddReader(MXFReader *reader);
    bool Finalize(bool check_is_complete, bool keep_input_order);

    void UpdateReadLimits();

public:
    virtual void GetAvailableReadLimits(int64_t *start_position, int64_t *duration) const;
    virtual void SetReadLimits();
    virtual void SetReadLimits(int64_t start_position, int64_t duration, bool seek_start_position);
    virtual int64_t GetReadStartPosition() const { return mReadStartPosition; }
    virtual int64_t GetReadDuration() const      { return mReadDuration; }

    virtual uint32_t Read(uint32_t num_samples, bool is_top = true);
    virtual void Seek(int64_t position);

    virtual int64_t GetPosition() const { return mPosition; }

    virtual int16_t GetMaxPrecharge(int64_t position, bool limit_to_available) const;
    virtual int16_t GetMaxRollout(int64_t position, bool limit_to_available) const;

public:
    virtual bool HaveFixedLeadFillerOffset() const;
    virtual int64_t GetFixedLeadFillerOffset() const;

public:
    virtual size_t GetNumTrackReaders() const { return mTrackReaders.size(); }
    virtual MXFTrackReader* GetTrackReader(size_t track_index) const;

    virtual bool IsEnabled() const;

    virtual int16_t GetTrackPrecharge(size_t track_index, int64_t clip_position, int16_t clip_precharge) const;
    virtual int16_t GetTrackRollout(size_t track_index, int64_t clip_position, int16_t clip_rollout) const;

public:
    virtual void SetNextFramePosition(int64_t position);
    virtual void SetNextFrameTrackPositions();

private:
    bool FindSequenceStart(const std::vector<MXFGroupReader*> &group_readers, size_t *seq_start_index) const;

    void GetSegmentPosition(int64_t position, MXFGroupReader **segment, size_t *segment_index,
                            int64_t *segment_position) const;

private:
    std::vector<bmx::MXFReader*> mReaders;
    std::vector<bmx::MXFGroupReader*> mGroupSegments;

    std::vector<bmx::MXFSequenceTrackReader*> mTrackReaders;

    int64_t mReadStartPosition;
    int64_t mReadDuration;

    std::vector<std::vector<uint32_t> > mSampleSequences;
    std::vector<int64_t> mSampleSequenceSizes;

    std::vector<int64_t> mSegmentOffsets;
    std::vector<int64_t> mSegmentOffsetAdjustments;

    int64_t mPosition;
};


};



#endif

