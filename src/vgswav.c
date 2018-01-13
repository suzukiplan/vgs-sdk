/* WAVE��Ǝ��`����PCM�f�[�^�ɕϊ����� */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ���w�b�_ */
struct DatHead {
	char riff[4];
	unsigned int fsize;
	char wave[4];
	char fmt[4];
	unsigned int bnum;
	unsigned short fid;
	unsigned short ch;
	unsigned int sample;
	unsigned int bps;
	unsigned short bsize;
	unsigned short bits;
	char data[4];
	unsigned int dsize;
};

int main(int argc,char* argv[])
{
	FILE* fpR=NULL;
	FILE* fpW=NULL;
	int rc=0;
	struct DatHead dh;
	char* data=NULL;
	char mh[4];

	/* �����`�F�b�N */
	rc++;
	if(argc<3) {
		fprintf(stderr,"usage: vgswav input(wav) output(pcm)\n");
		goto ENDPROC;
	}

	/* �ǂݍ��݃t�@�C�����I�[�v�� */
	rc++;
	if(NULL==(fpR=fopen(argv[1],"rb"))) {
		fprintf(stderr,"ERROR: Could not open: %s\n",argv[1]);
		goto ENDPROC;
	}

	/* ���w�b�_��ǂݍ��� */
	rc++;
	if(sizeof(dh)!=fread(&dh,1,sizeof(dh),fpR)) {
		fprintf(stderr,"ERROR: Invalid file header.\n");
		goto ENDPROC;
	}

	/* �`���`�F�b�N */
	rc++;
	if(0!=strncmp(dh.riff,"RIFF",4)) {
		fprintf(stderr,"ERROR: Not RIFF format.\n");
		goto ENDPROC;
	}
	rc++;
	if(0!=strncmp(dh.wave,"WAVE",4)) {
		fprintf(stderr,"ERROR: Not WAVE format.\n");
		goto ENDPROC;
	}
	rc++;
	if(0!=strncmp(dh.fmt,"fmt ",4)) {
		fprintf(stderr,"ERROR: Invalid format.\n");
		goto ENDPROC;
	}
	rc++;
	if(0!=strncmp(dh.data,"data",4)) {
		fprintf(stderr,"ERROR: Invalid data.\n");
		goto ENDPROC;
	}

	printf("Header of %s:\n",argv[1]);
	printf(" - Format: %d\n",dh.fid);
	printf(" - Channel: %dch\n",dh.ch);
	printf(" - Sample: %dHz\n",dh.sample);
	printf(" - Transform: %dbps\n",dh.bps);
	printf(" - Block-size: %dbyte\n",(int)dh.bsize);
	printf(" - Bit-rate: %dbit\n",(int)dh.bits);
	printf(" - PCM: %dbyte\n",(int)dh.dsize);


	rc++;
	if(22050!=dh.sample) {
		fprintf(stderr,"ERROR: Sampling rate is not 22050Hz.\n");
		goto ENDPROC;
	}
	rc++;
	if(1!=dh.ch) {
		fprintf(stderr,"ERROR: Sampling channel is not 1(mono).\n");
		goto ENDPROC;
	}
	rc++;
	if(16!=dh.bits) {
		fprintf(stderr,"ERROR: Sampling bit rate is not 16bit.\n");
		goto ENDPROC;
	}
	rc++;
	if(dh.sample*2!=dh.bps) {
		fprintf(stderr,"ERROR: Invalid transform-rate(byte/sec).\n");
		goto ENDPROC;
	}

	/* �g�`�f�[�^��ǂލ��ޗ̈���m�ۂ��� */
	rc++;
	if(NULL==(data=(char*)malloc(dh.dsize))) {
		fprintf(stderr,"ERROR: Memory allocation error.\n");
		goto ENDPROC;
	}

	/* �g�`�f�[�^��ǂݍ��� */
	rc++;
	if(dh.dsize!=fread(data,1,dh.dsize,fpR)) {
		fprintf(stderr,"ERROR: Could not read PCM data.\n");
		goto ENDPROC;
	}

	/* �������݃t�@�C�����I�[�v�� */
	rc++;
	if(NULL==(fpW=fopen(argv[2],"wb"))) {
		fprintf(stderr,"ERROR: Could not open: %s\n",argv[2]);
		goto ENDPROC;
	}

	/* �w�b�_�������� */
	rc++;
	strcpy(mh,"EFF");
	if(4!=fwrite(mh,1,4,fpW)) {
		fprintf(stderr,"ERROR: Could not write header.\n");
		goto ENDPROC;
	}

	/* �T�C�Y��񏑂�����(Big-endian) */
	rc++;
	mh[0]=((dh.dsize & 0xFF000000) >> 24) & 0xFF;
	mh[1]=((dh.dsize & 0x00FF0000) >> 16) & 0xFF;
	mh[2]=((dh.dsize & 0x0000FF00) >> 8) & 0xFF;
	mh[3]=dh.dsize & 0xFF;
	if(4!=fwrite(mh,1,4,fpW)) {
		fprintf(stderr,"ERROR: Could not write size.\n");
		goto ENDPROC;
	}

	/* PCM�������� */
	if(dh.dsize!=fwrite(data,1,dh.dsize,fpW)) {
		fprintf(stderr,"ERROR: Could not write data.\n");
		goto ENDPROC;
	}

	rc=0;

	/* �I������ */
ENDPROC:
	if(data) {
		free(data);
	}
	if(fpR) {
		fclose(fpR);
	}
	if(fpW) {
		fclose(fpW);
	}
	return rc;
}
