#pragma once
#include <fstream>
#include <cmath>
using namespace std;

double pi = acos(-1);

struct WavHeader {
	char chunkId[4]; // "RIFF"
	int chunkSize; // Total file size - 8
	char format[4]; // "WAVE"
	char subchunk1Id[4]; // "fmt "
	int subchunk1Size; // 16 for PCM
	short audioFormat; // 1 for PCM
	short numChannels; // 1 for Mono, 2 for Stereo
	int sampleRate; // e.g. 44100
	int byteRate; // sampleRate * numChannels * 2
	short blockAlign; // numChannels * 2
	short bitsPerSample; // 16
	char subchunk2Id[4]; // "data"
	int subchunk2Size; // Size of audio data in bytes
};

class complexNum {
private:
	double r, i;
public:
	complexNum() : i(0), r(0) {}
	complexNum(double re, double im) : i(im), r(re) {}
	double real() const { return r; }
	double imaginary() const { return i; }
	void setreal(double newr) { r = newr; }
	void setimaginary(double newi) { i = newi; }

	complexNum operator+(const complexNum& other) const {
		complexNum c(real() + other.real(), imaginary() + other.imaginary());
		return c;
	}
	complexNum operator-(const complexNum& other) const {
		complexNum c(real() - other.real(), imaginary() - other.imaginary());
		return c;
	}
	complexNum operator*(const complexNum& other) const {
		double a = r, b = i, c = other.real(), d = other.imaginary();
		double newr = a * c - b * d;
		double newi = a * d + b * c;
		complexNum newc(newr, newi);
		return newc;
	}
	complexNum operator*(double val) const {
		complexNum c(r * val, i * val);
		return c;
	}
	complexNum operator/(const complexNum& other) const {
		double a = r, b = i, c = other.real(), d = other.imaginary();
		double newr = (a * c + b * d) / (c * c + d * d);
		double newi = (b * c - a * d) / (c * c + d * d);
		complexNum newc(newr, newi);
		return newc;
	}
	complexNum conjugate() const {
		complexNum c(r, -i);
		return c;
	}
};

void FFT(complexNum*, int);
void IFFT(complexNum*, int);

class Spectrum {
public:
	complexNum* frequencyBins;
	int sampleRate;
	int count;
	int ogcount;
	Spectrum() {}
	Spectrum(int size) {
		this->count = size;
		frequencyBins = new complexNum[size];
	}
	~Spectrum() {
		if (frequencyBins) delete[] frequencyBins;
	}
	void applyFilter(float* weights) {
		for (int i = 0; i < count; i++)
			frequencyBins[i] = frequencyBins[i] * weights[i];
	}
};

class Filter {
public:
	float* weights;
	int size;

	Filter(int size) {
		this->size = size;
		weights = new float[size];
		for (int i = 0; i < size; i++) weights[i] = 1.0f;
	}

	~Filter() { delete[] weights; }

	int freqToIndex(float freq, int sampleRate, int fftSize) {
		return int(freq * fftSize / sampleRate);
	}
	void lowPass(float cutoff, int sampleRate) {
		int cutoffIndex = freqToIndex(cutoff, sampleRate, size);
		for (int i = cutoffIndex; i < size; i++) weights[i] = 0.0f;
	}
	void highPass(float cutoff, int sampleRate) {
		int cutoffIndex = freqToIndex(cutoff, sampleRate, size);
		for (int i = 0; i < cutoffIndex; i++) weights[i] = 0.0f;
	}
	void bandPass(float low, float high, int sampleRate) {
		int lowIndex = freqToIndex(low, sampleRate, size);
		int highIndex = freqToIndex(high, sampleRate, size);
		for (int i = 0; i < size; i++)
			if (i < lowIndex || i > highIndex) weights[i] = 0.0f;
	}
};


class Whisper {
public:
	WavHeader* header;
	short* buffer;
	int sampleCount;
	int ogcount;

	Whisper() {
		buffer = nullptr; header = nullptr; sampleCount = ogcount = 0; makePower2();
	}
	Whisper(const Whisper& whisp) {
		if (whisp.header)
			header = new WavHeader(*(whisp.header));
		sampleCount = whisp.sampleCount;
		ogcount = whisp.ogcount;
		buffer = new short[sampleCount];
		for (int i = 0; i < sampleCount; i++)
			buffer[i] = whisp.buffer[i];
	}
	Whisper(WavHeader* h) {
		buffer = nullptr;
		sampleCount = ogcount = 0;
		header = new WavHeader(*h);
	}
	Whisper(Spectrum* s) {
		sampleCount = s->ogcount;
		header = new WavHeader;
		buffer = new short[sampleCount];
		IFFT(s->frequencyBins, s->count);
		for (int i = 0; i < sampleCount; i++)
			buffer[i] = s->frequencyBins[i].real();
	}
	~Whisper() {
		if (header) delete header;
		if (buffer) delete[] buffer;
	}

	void hardclip() {
		for (int i = 0; i < sampleCount; i++) {
			buffer[i] = (buffer[i] > 32767 ? 32767 : buffer[i]);
			buffer[i] = (buffer[i] < -32768 ? -32768 : buffer[i]);
		}
	}

	void normalise() {
		if (buffer == nullptr || sampleCount == 0) return;
		int max = abs(buffer[0]);
		for (int i = 0; i < sampleCount; i++)
			if (abs(buffer[i]) > max) max = abs(buffer[i]);
		if (!max) return;
		double scale = 32767.0 / max;
		for (int i = 0; i < sampleCount; i++)
			buffer[i] = short(buffer[i] * scale);
		hardclip();
	}

	void operator*=(float factor) {
		for (int i = 0; i < sampleCount; i++)
			buffer[i] = short(buffer[i] * factor);
		hardclip();
	}

	void reverse() {
		for (int i = 0; i < sampleCount / 2; i++) {
			short temp = buffer[i];
			buffer[i] = buffer[sampleCount - i - 1];
			buffer[sampleCount - i - 1] = temp;
		}
	}

	Whisper* splice(double start, double end) const {
		int newstart = start * header->sampleRate;
		int newend = end * header->sampleRate;
		if (newstart >= newend) return nullptr;
		if (newstart < 0) newstart = 0;
		if (newend > sampleCount) newend = sampleCount;
		if (newstart > sampleCount) newstart = sampleCount;
		Whisper* w = new Whisper(header);
		w->sampleCount = newend - newstart;
		w->buffer = new short[w->sampleCount];
		for (int i = 0; i < w->sampleCount; i++)
			w->buffer[i] = buffer[newstart + i];
		if (w->header) {
			w->header->subchunk2Size = w->sampleCount * 2;
			w->header->chunkSize = 36 + w->header->subchunk2Size;
		}
		return w;
	}

	Whisper operator+(const Whisper& other) {
		int min = (sampleCount < other.sampleCount ? sampleCount : other.sampleCount);
		Whisper w;
		if (header) {
			w.header = new WavHeader(*header); w.header->subchunk2Size = min * 2;
			w.header->chunkSize = 36 + w.header->subchunk2Size;
		}
		w.sampleCount = min;
		w.buffer = new short[min];
		for (int i = 0; i < min; i++)
			w.buffer[i] = buffer[i] + other.buffer[i];
		w.hardclip();
		return w;
	}
	Whisper& operator+=(const Whisper& other) {
		int size = sampleCount + other.sampleCount;
		short* newbuff = new short[size];
		for (int i = 0; i < sampleCount; i++) newbuff[i] = buffer[i];
		for (int i = 0; i < other.sampleCount; i++)newbuff[sampleCount + i] = other.buffer[i];
		delete[] buffer; buffer = newbuff;
		sampleCount = size;
		if (header) {
			header->subchunk2Size = sampleCount * 2;
			header->chunkSize = 36 + header->subchunk2Size;
		}
		return *this;
	}

	void makePower2() {
		ogcount = sampleCount;
		if (!(sampleCount & (sampleCount - 1))) return;
		int nearest2 = 1;
		while (nearest2 < sampleCount) nearest2 *= 2;
		if (nearest2 == sampleCount) return;
		sampleCount = nearest2;
		int offset = sampleCount - ogcount;
		short* newbuffer = new short[sampleCount] {0};
		for (int i = 0; i < ogcount; i++)
			newbuffer[i] = buffer[i];
		delete[]buffer; buffer = newbuffer;
		if (header) {
			header->subchunk2Size = sampleCount * 2;
			header->chunkSize = 36 + header->subchunk2Size;
		}
	}

	void reversePower2() {
		short* newbuffer = new short[ogcount];
		for (int i = 0; i < ogcount; i++)
			newbuffer[i] = buffer[i];
		delete[]buffer; buffer = newbuffer;
		sampleCount = ogcount;
		if (header) {
			header->subchunk2Size = sampleCount * 2;
			header->chunkSize = 36 + header->subchunk2Size;
		}
	}

	Spectrum* makespec() {
		Spectrum* s = new Spectrum;
		s->sampleRate = header->sampleRate;
		makePower2();
		s->count = sampleCount;
		s->ogcount = ogcount;
		s->frequencyBins = new complexNum[s->count];
		for (int i = 0; i < s->count; i++)
			s->frequencyBins[i].setreal(buffer[i]);
		FFT(s->frequencyBins, s->count);
		return s;
	}

	Spectrum* toSpectrum() {

		makePower2();

		Spectrum* spec = new Spectrum(sampleCount);
		spec->sampleRate = header->sampleRate;
		spec->ogcount = ogcount;

		for (int i = 0; i < sampleCount; i++)
			spec->frequencyBins[i].setreal(buffer[i]);

		FFT(spec->frequencyBins, sampleCount);

		return spec;
	}

	void applyFilter(Filter* f) {
		Spectrum* s = toSpectrum();
		s->applyFilter(f->weights);
		IFFT(s->frequencyBins, s->count);
		reversePower2();
		for (int i = 0; i < sampleCount; i++) buffer[i] = short(s->frequencyBins[i].real());
		delete s;
	}
};

bool compare(const char* str1, const char* str2, int len) {
	for (int i = 0; i < len; i++)
		if (str1[i] != str2[i]) return false;
	return true;
}

void load(WavHeader*& w, Whisper*& whisp, const char* filename) {
	w = nullptr; whisp = nullptr;
	ifstream file(filename, ios::binary);
	if (!file) return;
	w = new WavHeader;
	file.read((char*)w, 40);
	char* temp = new char[1000]; int i = 0;
	char* ref = temp;
	while (file) {
		file.read(temp, 2);
		temp += 2; i += 2;
		if (i < 5) continue;
		if (compare((temp - 4), "data", 4)) {
			temp -= 4;
			for (int j = 0; j < 4; j++)
				w->subchunk2Id[j] = temp[j];
			delete[] ref;
			break;
		}
	}
	file.read((char*)&w->subchunk2Size, 4);
	whisp = new Whisper(w);

	switch (w->bitsPerSample) {
	case 16:
		whisp->sampleCount = w->subchunk2Size / 2;
		whisp->buffer = new short[whisp->sampleCount];
		file.read((char*)whisp->buffer, w->subchunk2Size);
		break;
	case 8:
		whisp->sampleCount = w->subchunk2Size;
		whisp->buffer = new short[whisp->sampleCount];
		unsigned char* temp = new unsigned char[whisp->sampleCount];
		file.read((char*)temp, w->subchunk2Size);
		for (int i = 0; i < whisp->sampleCount; i++)
			whisp->buffer[i] = ((int)temp[i] - 128) * 256;
		delete[] temp;  temp = nullptr;
	}
	whisp->ogcount = whisp->sampleCount;
}

void FFT(complexNum* freqs, int n) {
	if (n <= 1) return;

	complexNum* even = new complexNum[n / 2];
	complexNum* odd = new complexNum[n / 2];
	for (int i = 0, j = 0; i < n; i += 2, j++) {
		even[j] = freqs[i];
		odd[j] = freqs[i + 1];
	}

	FFT(even, n / 2);
	FFT(odd, n / 2);

	for (int k = 0; k < n / 2; k++) {
		double angle = -2 * pi * k / n;
		complexNum w(cos(angle), sin(angle));
		complexNum t = w * odd[k];
		freqs[k] = even[k] + t;
		freqs[k + n / 2] = even[k] - t;
	}

	delete[] even;
	delete[] odd;
}


void IFFT(complexNum* freqs, int n) {
	for (int i = 0; i < n; i++) freqs[i] = freqs[i].conjugate();
	FFT(freqs, n);
	for (int i = 0; i < n; i++) freqs[i] = freqs[i].conjugate() * (1.0 / n);
}


class Orchestrator {
public:
	Whisper* whispers[4];
	Spectrum* spectrums[4];
	int activeIndex;

	Orchestrator() {
		for (int i = 0; i < 4; i++) { whispers[i] = nullptr; spectrums[i] = nullptr; }
		activeIndex = -1;
	}

	~Orchestrator() {
		for (int i = 0; i < 4; i++) { delete whispers[i]; delete spectrums[i]; }
	}

	bool loadTrack(const char* filename) {
		for (int i = 0; i < 4; i++)
			if (!whispers[i]) {
				WavHeader* w = nullptr;
				Whisper* wh = nullptr;
				load(w, wh, filename);
				if (!wh) return false;
				whispers[i] = wh;
				activeIndex = i;
				return true;
			}
		return false;
	}

	void activate(int index) {
		if (index >= 0 && index < 4 && whispers[index]) activeIndex = index;
	}

	void applyGain(float factor) {
		if (activeIndex >= 0) (*whispers[activeIndex]) *= factor;
	}

	void reverseActive() {
		if (activeIndex >= 0) whispers[activeIndex]->reverse();
	}

	void filterActive(Filter* f) {
		if (activeIndex >= 0) whispers[activeIndex]->applyFilter(f);
	}

	void spectrumActive() {
		if (activeIndex >= 0) {
			if (spectrums[activeIndex]) delete spectrums[activeIndex];
			spectrums[activeIndex] = whispers[activeIndex]->toSpectrum();
		}
	}
};