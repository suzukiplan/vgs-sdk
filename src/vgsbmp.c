/* Windows��256�Fbitmap��Ǝ��`���ɕϊ�����c�[�� */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ���w�b�_ */
struct DatHead {
	int isize;					/* ���w�b�_�T�C�Y */
	int width;					/* �� */
	int height;					/* ���� */
	unsigned short planes;		/* �v���[���� */
	unsigned short bits;		/* �F�r�b�g�� */
	unsigned int ctype;			/* ���k�`�� */
	unsigned int gsize;			/* �摜�f�[�^�T�C�Y */
	int xppm;					/* X�����𑜓x */
	int yppm;					/* Y�����𑜓x */
	unsigned int cnum;			/* �g�p�F�� */
	unsigned int inum;			/* �d�v�F�� */
};

int main(int argc,char* argv[])
{
	FILE* fpR=NULL;
	FILE* fpW=NULL;
	int rc=0;
	char fh[14];
	int pal[256];
	struct DatHead dh;
	unsigned char r,g,b;
	unsigned char mh[4];
	int i;
	char* gbuf=NULL;

	/* �����`�F�b�N */
	rc++;
	if(argc<3) {
		fprintf(stderr,"usage: vgsbmp input(bmp) output(chr)\n");
		goto ENDPROC;
	}

	/* �ǂݍ��݃t�@�C�����I�[�v�� */
	rc++;
	if(NULL==(fpR=fopen(argv[1],"rb"))) {
		fprintf(stderr,"ERROR: Could not open: %s\n",argv[1]);
		goto ENDPROC;
	}

	/* �t�@�C���w�b�_��ǂݍ��� */
	rc++;
	if(sizeof(fh)!=fread(fh,1,sizeof(fh),fpR)) {
		fprintf(stderr,"ERROR: Invalid file header.\n");
		goto ENDPROC;
	}

	/* �擪2�o�C�g�����ǂ� */
	rc++;
	if(strncmp(fh,"BM",2)) {
		fprintf(stderr,"ERROR: Invalid file header.\n");
		goto ENDPROC;
	}

	/* ���w�b�_��ǂݍ��� */
	rc++;
	if(sizeof(dh)!=fread(&dh,1,sizeof(dh),fpR)) {
		fprintf(stderr,"ERROR: Invalid file header.\n");
		goto ENDPROC;
	}
#if 0
	printf("INPUT: width=%d, height=%d, bits=%d(%d), cmp=%d\n"
		,dh.width
		,dh.height
		,(int)dh.bits
		,dh.cnum
		,dh.ctype
		);
#endif

	/* ���y�э����́A1�s�N�Z���ȏ�256�s�N�Z���ȉ��̂� */
	rc++;
	if(dh.width<1 || 256<dh.width || dh.height<1 || 256<dh.height) {
		fprintf(stderr,"ERROR: Size too large. (MAX:256x256)\n");
		goto ENDPROC;
	}

	/* 8�r�b�g�J���[�ȊO�͒e�� */
	rc++;
	if(8!=dh.bits) {
		fprintf(stderr,"ERROR: This program supports only 8bit color.\n");
		goto ENDPROC;
	}

	/* �����k�ȊO�͒e�� */
	rc++;
	if(dh.ctype) {
		fprintf(stderr,"ERROR: This program supports only none-compress type.\n");
		goto ENDPROC;
	}

	/* �摜�f�[�^��ǂލ��ޗ̈���m�ۂ��� */
	rc++;
	if(NULL==(gbuf=(char*)malloc(dh.gsize))) {
		fprintf(stderr,"ERROR: This program supports only 8bit color.\n");
		goto ENDPROC;
	}

	/* �p���b�g����ǂݍ��� */
	rc++;
	if(sizeof(pal)!=fread(pal,1,sizeof(pal),fpR)) {
		fprintf(stderr,"ERROR: Could not read pallete data.\n");
		goto ENDPROC;
	}

	/* �摜�f�[�^��ǂݍ��� */
	rc++;
	if(dh.gsize!=fread(gbuf,1,dh.gsize,fpR)) {
		fprintf(stderr,"ERROR: Could not read graphic data.\n");
		goto ENDPROC;
	}

	/* �������݃t�@�C�����I�[�v�� */
	rc++;
	if(NULL==(fpW=fopen(argv[2],"wb"))) {
		fprintf(stderr,"ERROR: Could not open: %s\n",argv[2]);
		goto ENDPROC;
	}

	/* �w�b�_���������� */
	rc++;
	mh[0]='S';
	mh[1]='Y';
	mh[2]=(dh.width-1) & 0xFF;
	mh[3]=(dh.height-1) & 0xFF;
	if(sizeof(mh)!=fwrite(mh,1,sizeof(mh),fpW)) {
		fprintf(stderr,"ERROR: Could not write size data.\n");
		goto ENDPROC;
	}

	/* �摜�f�[�^���㉺���]���ď������� */
	/* (BMP�͏㉺���]��Ԃŏ�����Ă��邽��) */
	rc++;
	for(i=dh.height-1;0<=i;i--) {
		if(dh.width!=fwrite(&gbuf[i*dh.width],1,dh.width,fpW)) {
			fprintf(stderr,"ERROR: Could not write graphic data.\n");
			goto ENDPROC;
		}
	}

	rc=0;

	/* �I������ */
ENDPROC:
	if(gbuf) {
		free(gbuf);
	}
	if(fpR) {
		fclose(fpR);
	}
	if(fpW) {
		fclose(fpW);
	}
	return rc;
}
