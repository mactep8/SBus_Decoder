using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace SBUSDecoderCFG
{
    public partial class frmMain : Form
    {
        Dictionary<int, ComboBox> cbBankA = new Dictionary<int,ComboBox>();
        Dictionary<int, ComboBox> cbBankB = new Dictionary<int, ComboBox>();

        private void cbBanksInit()
        {
            cbBankA.Clear();
            cbBankA.Add(1, cbChA1);
            cbBankA.Add(2, cbChA2);
            cbBankA.Add(3, cbChA3);
            cbBankA.Add(4, cbChA4);
            cbBankA.Add(5, cbChA5);
            cbBankA.Add(6, cbChA6);
            cbBankA.Add(7, cbChA7);
            cbBankA.Add(8, cbChA8);
            cbBankA.Add(9, cbChA9);
            cbBankA.Add(10, cbChA10);
            cbBankA.Add(11, cbChA11);
            cbBankA.Add(12, cbChA12);
            cbBankA.Add(13, cbChA13);
            cbBankA.Add(14, cbChA14);
            cbBankA.Add(15, cbChA15);
            cbBankA.Add(16, cbChA16);

            cbBankB.Clear();
            cbBankB.Add(1, cbChB1);
            cbBankB.Add(2, cbChB2);
            cbBankB.Add(3, cbChB3);
            cbBankB.Add(4, cbChB4);
            cbBankB.Add(5, cbChB5);
            cbBankB.Add(6, cbChB6);
            cbBankB.Add(7, cbChB7);
            cbBankB.Add(8, cbChB8);
            cbBankB.Add(9, cbChB9);
            cbBankB.Add(10, cbChB10);
            cbBankB.Add(11, cbChB11);
            cbBankB.Add(12, cbChB12);
            cbBankB.Add(13, cbChB13);
            cbBankB.Add(14, cbChB14);
            cbBankB.Add(15, cbChB15);
            cbBankB.Add(16, cbChB16);

            SetBankDefaultCh(cbBankA);
            SetBankDefaultCh(cbBankB);
        }

        public frmMain()
        {
            InitializeComponent();
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            cbBanksInit();
            cbPorts.Items.Clear();
            cbPorts.Items.AddRange(SerialPort.GetPortNames());
            if (cbPorts.Items.Count > 0) cbPorts.SelectedIndex = 0;
        }

        private int GetChannelsCount(int FrameIndex)
        {
            int ms = (FrameIndex * 4) + 16;
            double us = ms * 1000;
            // minus sync
            us -= 2700;
            // 2200us per channel
            int chc = Convert.ToInt32(Math.Floor(us / 2200));
            return chc;
        }

        void update_cbChannelsPPM(ComboBox cb, int FrameIndex, int maxCh)
        {
            int maxChannelsCount = GetChannelsCount(FrameIndex);
            if (maxChannelsCount > maxCh) maxChannelsCount = maxCh;
            cb.Items.Clear();
            for (int i = 1; i <= maxChannelsCount; i++)
                cb.Items.Add(i);
            // Default = maxChannelsCount
            cb.SelectedIndex = maxChannelsCount - 1;
        }

        void update_cbChannelsPWM(ComboBox cb, int FrameIndex, int maxCh)
        {
            int maxChannelsCount = GetChannelsCount(FrameIndex)+1;
            if (maxChannelsCount > maxCh) maxChannelsCount = maxCh;
            cb.Items.Clear();
            for (int i = 1; i <= maxChannelsCount; i++)
                cb.Items.Add(i);
            // Default = maxChannelsCount
            cb.SelectedIndex = maxChannelsCount - 1;
        }

        private void UpdateBankEnabled(Dictionary<int, ComboBox> Bank, int chCount)
        {
            foreach (KeyValuePair<int, ComboBox> itm in Bank)
                itm.Value.Enabled = itm.Key <= chCount;
        }

        private void SetBankDefaultCh(Dictionary<int, ComboBox> Bank)
        {
            foreach (KeyValuePair<int, ComboBox> itm in Bank)
                itm.Value.SelectedIndex = itm.Key-1;
        }

        private void chkPPMB_CheckedChanged(object sender, EventArgs e)
        {
            if (!chkPPMB.Checked)
            {
                // Frame = 20ms
                cbFrameB.SelectedIndex = 1;
                // Channels <8
                update_cbChannelsPWM(cbChannelsB, cbFrameB.SelectedIndex, 8);
            }
            else
            {
                // Frame = 24ms
                cbFrameB.SelectedIndex = 2;
                // Channels up to 16
                update_cbChannelsPPM(cbChannelsB, cbFrameB.SelectedIndex, 16);
            }
            UpdateBankEnabled(cbBankB, cbChannelsB.Items.Count);
        }

        private void chkPPMA_CheckedChanged(object sender, EventArgs e)
        {
            if (!chkPPMA.Checked)
            {
                // Frame = 20ms
                cbFrameA.SelectedIndex = 1;
                // Channels <8
                update_cbChannelsPWM(cbChannelsA, cbFrameA.SelectedIndex, 8);
            }
            else
            {
                // Frame = 24ms
                cbFrameA.SelectedIndex = 2;
                // Channels up to 16
                update_cbChannelsPPM(cbChannelsA, cbFrameA.SelectedIndex, 16);
            }
            UpdateBankEnabled(cbBankA, cbChannelsA.Items.Count);
        }

        private void btnRead_Click(object sender, EventArgs e)
        {
            byte[] buff = new byte[41];
            buff[0] = 0xaa;
            buff[1] = 0x52; // R

            for (int i = 2; i < 40; i++)
                buff[i] = 0;

            buff[40] = 0x5d;

            COMPort.Open();
            COMPort.Write(buff, 0, 41);

            int cnt = 0;
            while (cnt < 40)
            {
                if (COMPort.BytesToRead > 40) 
                    cnt = COMPort.Read(buff, 0, 41);
            }
            COMPort.Close();

            chkPPMA.Checked = (buff[2] == 1);
            if (buff[3] - 4 < 7) cbFrameA.SelectedIndex = buff[3] - 4;
            if (buff[4] - 1 < 16) cbChannelsA.SelectedIndex = buff[4] - 1;

            if (buff[5] < 17) cbChA1.SelectedIndex = buff[5];
            if (buff[6] < 17) cbChA2.SelectedIndex = buff[6];
            if (buff[7] < 17) cbChA3.SelectedIndex = buff[7];
            if (buff[8] < 17) cbChA4.SelectedIndex = buff[8];
            if (buff[9] < 17) cbChA5.SelectedIndex = buff[9];
            if (buff[10] < 17) cbChA6.SelectedIndex = buff[10];
            if (buff[11] < 17) cbChA7.SelectedIndex = buff[11];
            if (buff[12] < 17) cbChA8.SelectedIndex = buff[12];
            if (buff[13] < 17) cbChA9.SelectedIndex = buff[13];
            if (buff[14] < 17) cbChA10.SelectedIndex = buff[14];
            if (buff[15] < 17) cbChA11.SelectedIndex = buff[15];
            if (buff[16] < 17) cbChA12.SelectedIndex = buff[16];
            if (buff[17] < 17) cbChA13.SelectedIndex = buff[17];
            if (buff[18] < 17) cbChA14.SelectedIndex = buff[18];
            if (buff[19] < 17) cbChA15.SelectedIndex = buff[19];
            if (buff[20] < 17) cbChA16.SelectedIndex = buff[20];

            chkPPMB.Checked = (buff[21] == 1);
            if (buff[22] - 4 < 7) cbFrameB.SelectedIndex = buff[22] - 4;
            if (buff[23] - 1 < 16) cbChannelsB.SelectedIndex = buff[23] - 1;

            if (buff[24] < 17) cbChB1.SelectedIndex = buff[24];
            if (buff[25] < 17) cbChB2.SelectedIndex = buff[25];
            if (buff[26] < 17) cbChB3.SelectedIndex = buff[26];
            if (buff[27] < 17) cbChB4.SelectedIndex = buff[27];
            if (buff[28] < 17) cbChB5.SelectedIndex = buff[28];
            if (buff[29] < 17) cbChB6.SelectedIndex = buff[29];
            if (buff[30] < 17) cbChB7.SelectedIndex = buff[30];
            if (buff[31] < 17) cbChB8.SelectedIndex = buff[31];
            if (buff[32] < 17) cbChB9.SelectedIndex = buff[32];
            if (buff[33] < 17) cbChB10.SelectedIndex = buff[33];
            if (buff[34] < 17) cbChB11.SelectedIndex = buff[34];
            if (buff[35] < 17) cbChB12.SelectedIndex = buff[35];
            if (buff[36] < 17) cbChB13.SelectedIndex = buff[36];
            if (buff[37] < 17) cbChB14.SelectedIndex = buff[37];
            if (buff[38] < 17) cbChB15.SelectedIndex = buff[38];
            if (buff[39] < 17) cbChB16.SelectedIndex = buff[39];
        }

        private void btnWrite_Click(object sender, EventArgs e)
        {
            byte[] buff = new byte[41];
            buff[0] = 0xaa;
            buff[1] = 0x57; // W
            buff[2] = Convert.ToByte(chkPPMA.Checked);
            buff[3] = (byte)(cbFrameA.SelectedIndex + 4);
            buff[4] = (byte)(cbChannelsA.SelectedIndex + 1);

            buff[5] = (byte)(cbChA1.SelectedIndex);
            buff[6] = (byte)(cbChA2.SelectedIndex);
            buff[7] = (byte)(cbChA3.SelectedIndex);
            buff[8] = (byte)(cbChA4.SelectedIndex);
            buff[9] = (byte)(cbChA5.SelectedIndex);
            buff[10] = (byte)(cbChA6.SelectedIndex);
            buff[11] = (byte)(cbChA7.SelectedIndex);
            buff[12] = (byte)(cbChA8.SelectedIndex);
            buff[13] = (byte)(cbChA9.SelectedIndex);
            buff[14] = (byte)(cbChA10.SelectedIndex);
            buff[15] = (byte)(cbChA11.SelectedIndex);
            buff[16] = (byte)(cbChA12.SelectedIndex);
            buff[17] = (byte)(cbChA13.SelectedIndex);
            buff[18] = (byte)(cbChA14.SelectedIndex);
            buff[19] = (byte)(cbChA15.SelectedIndex);
            buff[20] = (byte)(cbChA16.SelectedIndex);

            buff[21] = Convert.ToByte(chkPPMB.Checked);
            buff[22] = (byte)(cbFrameB.SelectedIndex + 4);
            buff[23] = (byte)(cbChannelsB.SelectedIndex + 1);

            buff[24] = (byte)(cbChB1.SelectedIndex);
            buff[25] = (byte)(cbChB2.SelectedIndex);
            buff[26] = (byte)(cbChB3.SelectedIndex);
            buff[27] = (byte)(cbChB4.SelectedIndex);
            buff[28] = (byte)(cbChB5.SelectedIndex);
            buff[29] = (byte)(cbChB6.SelectedIndex);
            buff[30] = (byte)(cbChB7.SelectedIndex);
            buff[31] = (byte)(cbChB8.SelectedIndex);
            buff[32] = (byte)(cbChB9.SelectedIndex);
            buff[33] = (byte)(cbChB10.SelectedIndex);
            buff[34] = (byte)(cbChB11.SelectedIndex);
            buff[35] = (byte)(cbChB12.SelectedIndex);
            buff[36] = (byte)(cbChB13.SelectedIndex);
            buff[37] = (byte)(cbChB14.SelectedIndex);
            buff[38] = (byte)(cbChB15.SelectedIndex);
            buff[39] = (byte)(cbChB16.SelectedIndex);

            buff[40] = 0x5d;

            COMPort.Open();
            COMPort.Write(buff, 0, 41);
            COMPort.Close();
        }

        private void cbFrameA_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!chkPPMA.Checked)
            {
                // Channels <8
                update_cbChannelsPWM(cbChannelsA, cbFrameA.SelectedIndex, 8);
            }
            else
            {
                // Channels up to 16
                update_cbChannelsPPM(cbChannelsA, cbFrameA.SelectedIndex, 16);
            }
            UpdateBankEnabled(cbBankA, cbChannelsA.Items.Count);
        }

        private void cbFrameB_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!chkPPMB.Checked)
            {
                // Channels <8
                update_cbChannelsPWM(cbChannelsB, cbFrameB.SelectedIndex, 8);
            }
            else
            {
                // Channels up to 16
                update_cbChannelsPPM(cbChannelsB, cbFrameB.SelectedIndex, 16);
            }
            UpdateBankEnabled(cbBankB, cbChannelsB.Items.Count);
        }

        private void cbPorts_SelectedIndexChanged(object sender, EventArgs e)
        {
            COMPort.PortName = cbPorts.SelectedItem.ToString();
        }

    }
}
