#include "Wave.h"
#include "../../Audio/AudioTypes.h"
#include <fstream>


namespace GameDev2D
{
	bool Wave::LoadFromPath(const std::string& aPath, std::shared_ptr<WaveData>& aWaveData)
	{
		//Open the input file
		std::ifstream inFile(aPath.c_str(), std::ios::binary | std::ios::in);
		if (inFile.bad())
		{
			return false;
		}

		unsigned long dwChunkId = 0;
		unsigned long dwFileSize = 0; 
		unsigned long dwChunkSize = 0; 
		unsigned long dwExtra = 0;

		//look for 'RIFF' chunk identifier
		inFile.seekg(0, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
		if (dwChunkId != 'FFIR')
		{
			inFile.close();
			return false;
		}
		inFile.seekg(4, std::ios::beg); //get file size
		inFile.read(reinterpret_cast<char*>(&dwFileSize), sizeof(dwFileSize));
		if (dwFileSize <= 16)
		{
			inFile.close();
			return false;
		}
		inFile.seekg(8, std::ios::beg); //get file format
		inFile.read(reinterpret_cast<char*>(&dwExtra), sizeof(dwExtra));
		if (dwExtra != 'EVAW')
		{
			inFile.close();
			return false;
		}

		//look for 'fmt ' chunk id
		bool bFilledFormat = false;
		for (unsigned int i = 12; i < dwFileSize; )
		{
			inFile.seekg(i, std::ios::beg);
			inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
			inFile.seekg(i + 4, std::ios::beg);
			inFile.read(reinterpret_cast<char*>(&dwChunkSize), sizeof(dwChunkSize));
			if (dwChunkId == ' tmf')
			{
				inFile.seekg(i + 8, std::ios::beg);
				inFile.read(reinterpret_cast<char*>(&aWaveData->waveFormat), sizeof(aWaveData->waveFormat));
				bFilledFormat = true;
				break;
			}
			dwChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
			dwChunkSize += 1;
			dwChunkSize &= 0xfffffffe; //guarantees WORD padding alignment
			i += dwChunkSize;
		}

		if (bFilledFormat == false)
		{
			inFile.close();
			return false;
		}

		//look for 'data' chunk id
		bool bFilledData = false;
		for (unsigned int i = 12; i < dwFileSize; )
		{
			inFile.seekg(i, std::ios::beg);
			inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
			inFile.seekg(i + 4, std::ios::beg);
			inFile.read(reinterpret_cast<char*>(&dwChunkSize), sizeof(dwChunkSize));
			if (dwChunkId == 'atad')
			{
				aWaveData->data = new unsigned char[dwChunkSize];
				inFile.seekg(i + 8, std::ios::beg);
				inFile.read(reinterpret_cast<char*>(aWaveData->data), dwChunkSize);
				aWaveData->buffer.AudioBytes = dwChunkSize;
				aWaveData->buffer.pAudioData = (BYTE*)aWaveData->data;
				aWaveData->buffer.PlayBegin = 0;
				aWaveData->buffer.PlayLength = 0;
				bFilledData = true;
				break;
			}
			dwChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
			dwChunkSize += 1;
			dwChunkSize &= 0xfffffffe; //guarantees WORD padding alignment
			i += dwChunkSize;
		}
		if (!bFilledData)
		{
			inFile.close();
			return false;
		}

		//Close the input file
		inFile.close();

		//Return true
		return true;
	}
}